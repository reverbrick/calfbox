/*
Calf Box, an open source musical instrument.
Copyright (C) 2010-2013 Krzysztof Foltman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CBOX_PREFETCH_PIPE_H
#define CBOX_PREFETCH_PIPE_H

#include <assert.h>
#include <glib.h>
#include <pthread.h>
#include <sndfile.h>
#include <stdint.h>

struct cbox_waveform;

enum cbox_prefetch_pipe_state
{
    pps_free,
    pps_opening,
    pps_active,
    pps_finished,
    pps_error,
    pps_closing,
    pps_closed,
    pps_exit_thread,
};

struct cbox_prefetch_pipe
{
    enum cbox_prefetch_pipe_state state;
    int next_free_pipe;
    struct cbox_waveform *waveform;
    int16_t *data;
    uint32_t buffer_size;
    SF_INFO info;
    SNDFILE *sndfile;
    uint32_t file_pos_frame;
    uint32_t file_loop_start;
    uint32_t file_loop_end;
    uint32_t buffer_loop_end;
    size_t write_ptr;
    size_t produced;
    size_t consumed;
    gboolean finished;
    gboolean returned;
};

extern void cbox_prefetch_pipe_init(struct cbox_prefetch_pipe *pipe, uint32_t buffer_size);
extern void cbox_prefetch_pipe_consumed(struct cbox_prefetch_pipe *pipe, uint32_t frames);
extern void cbox_prefetch_pipe_close(struct cbox_prefetch_pipe *pipe);

static inline uint32_t cbox_prefetch_pipe_get_remaining(struct cbox_prefetch_pipe *pipe)
{
    assert(pipe->consumed <= pipe->produced);
    return pipe->produced - pipe->consumed;
}

struct cbox_prefetch_stack
{
    struct cbox_prefetch_pipe *pipes;
    int pipe_count;
    pthread_t thr_prefetch;
    int last_free_pipe;
};

extern struct cbox_prefetch_stack *cbox_prefetch_stack_new(int npipes, uint32_t buffer_size);
extern struct cbox_prefetch_pipe *cbox_prefetch_stack_pop(struct cbox_prefetch_stack *stack, struct cbox_waveform *waveform, uint32_t file_loop_start, uint32_t file_loop_end);
extern void cbox_prefetch_stack_push(struct cbox_prefetch_stack *stack, struct cbox_prefetch_pipe *pipe);
extern void cbox_prefetch_stack_destroy(struct cbox_prefetch_stack *stack);

#endif
