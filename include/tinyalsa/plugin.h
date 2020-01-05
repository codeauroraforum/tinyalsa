/* plugin.h
** Copyright (c) 2019, The Linux Foundation.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above
**     copyright notice, this list of conditions and the following
**     disclaimer in the documentation and/or other materials provided
**     with the distribution.
**   * Neither the name of The Linux Foundation nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
** WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
** ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
** BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
** BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
** WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
** OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
** IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#ifndef TINYALSA_PLUGIN_H
#define TINYALSA_PLUGIN_H

#include <stdint.h>
#include <stdlib.h>

#include <sound/asound.h>

/* static initializers */

#define SND_VALUE_ENUM(etexts, eitems)    \
    {.texts = etexts, .items = eitems}

#define SND_VALUE_BYTES(csize)    \
    {.size = csize }

#define SND_VALUE_INTEGER(icount, imin, imax, istep) \
    {.count = icount, .min = imin, .max = imax, .step = istep }

#define SND_VALUE_TLV_BYTES(csize, cget, cput)       \
    {.size = csize, .get = cget, .put = cput }

#define SND_CONTROL_ENUM(cname, cget, cput, cenum, priv_val, priv_data)   \
    {    .iface = SNDRV_CTL_ELEM_IFACE_MIXER,                             \
        .access = SNDRV_CTL_ELEM_ACCESS_READWRITE,                        \
        .type = SNDRV_CTL_ELEM_TYPE_ENUMERATED,                           \
        .name = cname, .value = &cenum, .get = cget, .put = cput,         \
        .private_value = priv_val, .private_data = priv_data,             \
    }

#define SND_CONTROL_BYTES(cname, cget, cput, cbytes, priv_val, priv_data) \
    {                                                                     \
        .iface = SNDRV_CTL_ELEM_IFACE_MIXER,                              \
        .access = SNDRV_CTL_ELEM_ACCESS_READWRITE,                        \
        .type = SNDRV_CTL_ELEM_TYPE_BYTES,                                \
        .name = cname, .value = &cbytes, .get = cget, .put = cput,        \
        .private_value = priv_val, .private_data = priv_data,             \
    }

#define SND_CONTROL_INTEGER(cname, cget, cput, cint, priv_val, priv_data) \
    {                                                                        \
        .iface = SNDRV_CTL_ELEM_IFACE_MIXER,                                 \
        .access = SNDRV_CTL_ELEM_ACCESS_READWRITE,                           \
        .type = SNDRV_CTL_ELEM_TYPE_INTEGER,                                 \
        .name = cname, .value = &cint, .get = cget, .put = cput,             \
        .private_value = priv_val, .private_data = priv_data,                \
    }

#define SND_CONTROL_TLV_BYTES(cname, cbytes, priv_val, priv_data)  \
    {                                                                        \
        .iface = SNDRV_CTL_ELEM_IFACE_MIXER,                                 \
        .access = SNDRV_CTL_ELEM_ACCESS_TLV_READWRITE,                       \
        .type = SNDRV_CTL_ELEM_TYPE_BYTES,                                   \
        .name = cname, .value = &cbytes,                                     \
        .private_value = priv_val, .private_data = priv_data,                \
    }

struct mixer_plugin;
struct pcm_plugin;
struct snd_node;

/** Operations that are required to be registered by the plugin.
 * @ingroup libtinyalsa-pcm
 */
struct pcm_plugin_ops {
    /** close the pcm plugin */
    int (*close) (struct pcm_plugin *plugin);
    /** Set the PCM hardware parameters to the plugin */
    int (*hw_params) (struct pcm_plugin *plugin,
                      struct snd_pcm_hw_params *params);
    /** Set the PCM software parameters to the plugin */
    int (*sw_params) (struct pcm_plugin *plugin,
                      struct snd_pcm_sw_params *params);
    /** Synchronize the pointer */
    int (*sync_ptr) (struct pcm_plugin *plugin,
                     struct snd_pcm_sync_ptr *sync_ptr);
    /** Write frames to plugin to be rendered to output */
    int (*writei_frames) (struct pcm_plugin *plugin,
                          struct snd_xferi *x);
    /** Read frames from plugin captured from input */
    int (*readi_frames) (struct pcm_plugin *plugin,
                         struct snd_xferi *x);
    /** Obtain the timestamp for the PCM */
    int (*ttstamp) (struct pcm_plugin *plugin,
                    int *tstamp);
    /** Prepare the plugin for data transfer */
    int (*prepare) (struct pcm_plugin *plugin);
    /** Start data transfer from/to the plugin */
    int (*start) (struct pcm_plugin *plugin);
    /** Drop pcm frames */
    int (*drop) (struct pcm_plugin *plugin);
    /** Any custom or alsa specific ioctl implementation */
    int (*ioctl) (struct pcm_plugin *plugin,
                  int cmd, void *arg);
};

/** Minimum and maximum values for hardware parameter constraints.
 * @ingroup libtinyalsa-pcm
 */
struct pcm_plugin_min_max {
    /** Minimum value for the hardware parameter */
    unsigned int min;
    /** Maximum value for the hardware parameter */
    unsigned int max;
};

/** Encapsulate the hardware parameter constraints
 * @ingroup libtinyalsa-pcm
 */
struct pcm_plugin_hw_constraints {
    /** Value for SNDRV_PCM_HW_PARAM_ACCESS param */
    uint64_t access;
    /** Value for SNDRV_PCM_HW_PARAM_FORMAT param.
     * As of this implementation ALSA supports 52 formats */
    uint64_t format;
    /** Value for SNDRV_PCM_HW_PARAM_SAMPLE_BITS param */
    struct pcm_plugin_min_max bit_width;
    /** Value for SNDRV_PCM_HW_PARAM_CHANNELS param */
    struct pcm_plugin_min_max channels;
    /** Value for SNDRV_PCM_HW_PARAM_RATE param */
    struct pcm_plugin_min_max rate;
    /** Value for SNDRV_PCM_HW_PARAM_PERIODS param */
    struct pcm_plugin_min_max periods;
    /** Value for SNDRV_PCM_HW_PARAM_PERIOD_BYTES param */
    struct pcm_plugin_min_max period_bytes;
};

struct pcm_plugin {
    /** Card number for the pcm device */
    unsigned int card;
    /** pointer to plugin operation */
    struct pcm_plugin_ops *ops;
    /** pointer to the contraints registered by the plugin */
    struct pcm_plugin_hw_constraints *constraints;
    /** pointer to pcm node under snd card definition */
    struct snd_node *node;
    /** Indicates read/write mode, etc.. */
    int mode;
    /* Pointer to hold the plugin's private data */
    void *priv;
    /* Tracks the plugin state */
    unsigned int state;
};

typedef void (*mixer_event_callback)(struct mixer_plugin *);

struct mixer_plugin_ops {
    void (*close) (struct mixer_plugin **plugin);
    int (*subscribe_events) (struct mixer_plugin *plugin,
                             mixer_event_callback event_cb);
    ssize_t (*read_event) (struct mixer_plugin *plugin,
                           struct snd_ctl_event *ev, size_t size);
};

struct snd_control {
    snd_ctl_elem_iface_t iface;
    unsigned int access;
    const char *name;
    snd_ctl_elem_type_t type;
    void *value;
    int (*get) (struct mixer_plugin *plugin,
                struct snd_control *control,
                struct snd_ctl_elem_value *ev);
    int (*put) (struct mixer_plugin *plugin,
                struct snd_control *control,
                struct snd_ctl_elem_value *ev);
    uint32_t private_value;
    void *private_data;
};

struct mixer_plugin {
    unsigned int card;
    struct mixer_plugin_ops *ops;
    void *priv;

    int eventfd;
    int subscribed;
    int event_cnt;

    struct snd_control *controls;
    unsigned int num_controls;
};

struct snd_value_enum {
    unsigned int items;
    char **texts;
};

struct snd_value_bytes {
    unsigned int size;
};

struct snd_value_tlv_bytes {
    unsigned int size;
    int (*get) (struct mixer_plugin *plugin,
                struct snd_control *control,
                struct snd_ctl_tlv *tlv);
    int (*put) (struct mixer_plugin *plugin,
                struct snd_control *control,
                struct snd_ctl_tlv *tlv);
};

struct snd_value_int {
    unsigned int count;
    int min;
    int max;
    int step;
};

/** Operations defined by the plugin.
 * */
struct snd_node_ops {
    /** Function pointer to get card definition */
    void* (*open_card)(unsigned int card);
    /** Function pointer to release card definition */
    void (*close_card)(void *card);
    /** Function pointer to get PCM definition */
    void* (*get_pcm)(void *card, unsigned int id);
    /** Function pointer to get mixer definition */
    void* (*get_mixer)(void *card);
    /** Get interger type properties from device definition */
    int (*get_int)(void *node, const char *prop, int *val);
    /** Get string type properties from device definition */
    int (*get_str)(void *node, const char *prop, char **val);
};

#endif /* end of TINYALSA_PLUGIN_H */
