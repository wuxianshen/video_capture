////////////////////////////////////////////////////////////////////////////////
// CONFIDENTIAL and PROPRIETARY software of Magewell Electronics Co., Ltd.
// Copyright (c) 2011-2014 Magewell Electronics Co., Ltd. (Nanjing)
// All rights reserved.
// This copyright notice MUST be reproduced on all authorized copies.
////////////////////////////////////////////////////////////////////////////////
#ifndef __MW_LINUX__
#define __MW_LINUX__

#define MW_MAX_PROCESS_NAME_LEN (64)

#include <sys/types.h>
#include "MWCaptureExtension.h"

#pragma pack(push)
#pragma pack(1)

enum {
    GET_STREAMS_COUNT_NUM                  = 1,
    GET_STREAMS_INFO_NUM,
    GET_CTRL_STREAM_ID_NUM,
    SET_CTRL_STREAM_ID_NUM,
    GET_SELF_STREAM_ID_NUM,

    // Product informations
    GET_CHANNEL_INFO_NUM,
    GET_FAMILY_INFO_NUM,

    GET_VIDEO_CAPS_NUM,
    GET_AUDIO_CAPS_NUM,

    // Firmware upgrade
    GET_FIRMWARE_STORAGE_NUM,
    SET_FIRMWARE_ERASE_NUM,
    SET_FIRMWARE_DATA_NUM,
    GET_FIRMWARE_DATA_NUM,

    // Device misc controls
    SET_LED_MODE_NUM,
    SET_POST_RECONFIG_NUM,

    // Temperature
    GET_CORE_TEMPERATURE_NUM,

    // Hardware timer
    GET_TIME_NUM,
    SET_TIME_NUM,
    TIME_REGULATION_NUM,

    TIMER_REGISTRATION_NUM,
    TIMER_DEREGISTRATION_NUM,
    TIMER_EXPIRE_TIME_NUM,
    TIMER_WAIT_NUM,
    TIMER_CANCEL_NUM,

    // Notifications
    NOTIFY_REGISTRATION_NUM,
    NOTIFY_DEREGISTRATION_NUM,
    NOTIFY_STATUS_NUM,
    NOTIFY_WAIT_NUM,
    NOTIFY_CANCEL_NUM,
    NOTIFY_ENABLE_NUM,

    // Video frame information
    GET_VIDEO_BUFFER_INFO_NUM,
    GET_VIDEO_FRAME_INFO_NUM,

    // Video capture
    VIDEO_CAPTURE_OPEN_NUM,
    VIDEO_CAPTURE_FRAME_NUM,
    VIDEO_CAPTURE_STATUS_NUM,
    VIDEO_CAPTURE_CLOSE_NUM,

    // Audio capture
    AUDIO_CAPTURE_OPEN_NUM,
    AUDIO_CAPTURE_FRAME_NUM,
    AUDIO_CAPTURE_CLOSE_NUM,

    // Input source
    VIDEO_INPUT_SOURCE_COUNT_NUM,
    VIDEO_INPUT_SOURCE_ARRAY_NUM,

    AUDIO_INPUT_SOURCE_COUNT_NUM,
    AUDIO_INPUT_SOURCE_ARRAY_NUM,

    SET_INPUT_SOURCE_SCAN_NUM,
    GET_INPUT_SOURCE_SCAN_NUM,
    GET_INPUT_SOURCE_SCAN_STATE_NUM,
    SET_AV_INPUT_SOURCE_LINK_NUM,
    GET_AV_INPUT_SOURCE_LINK_NUM,
    SET_VIDEO_INPUT_SOURCE_NUM,
    GET_VIDEO_INPUT_SOURCE_NUM,
    SET_AUDIO_INPUT_SOURCE_NUM,
    GET_AUDIO_INPUT_SOURCE_NUM,

    // EDID
    GET_EDID_DATA_NUM,
    SET_EDID_DATA_NUM,

    // Signal status
    GET_INPUT_SPECIFIC_STATUS_NUM,
    GET_VIDEO_SIGNAL_STATUS_NUM,
    GET_AUDIO_SIGNAL_STATUS_NUM,

    // HDMI status
    GET_HDMI_INFOFRAME_VALID_NUM,
    GET_HDMI_INFOFRAME_PACKET_NUM,

    // Video processing
    GET_VIDEO_INPUT_ASPECT_RATIO_NUM,
    SET_VIDEO_INPUT_ASPECT_RATIO_NUM,
    GET_VIDEO_INPUT_COLOR_FORMAT_NUM,
    SET_VIDEO_INPUT_COLOR_FORMAT_NUM,
    GET_VIDEO_INPUT_QUANTIZATION_RANGE_NUM,
    SET_VIDEO_INPUT_QUANTIZATION_RANGE_NUM,

    // VGA/Component timings
    GET_VIDEO_AUTO_H_ALIGN_NUM,
    SET_VIDEO_AUTO_H_ALIGN_NUM,
    GET_VIDEO_SAMPLING_PHASE_NUM,
    SET_VIDEO_SAMPLING_PHASE_NUM,
    GET_VIDEO_SAMPLING_PHASE_AUTO_NUM,
    SET_VIDEO_SAMPLING_PHASE_AUTO_NUM,
    SET_VIDEO_TIMING_NUM,
    GET_VIDEO_PREFERRED_TIMING_ARRAY_NUM,
    SET_VIDEO_CUSTOM_TIMING_NUM,
    GET_VIDEO_CUSTOM_TIMING_COUNT_NUM,
    GET_VIDEO_CUSTOM_TIMING_ARRAY_NUM,
    SET_VIDEO_CUSTOM_TIMING_ARRAY_NUM,
    GET_VIDEO_CUSTOM_RESOLUTION_COUNT_NUM,
    GET_VIDEO_CUSTOM_RESOLUTION_ARRAY_NUM,
    SET_VIDEO_CUSTOM_RESOLUTION_ARRAY_NUM,

    // Image upload, can be use as source image or OSD image
    VIDEO_CREATE_IMAGE_NUM,
    VIDEO_OPEN_IMAGE_NUM,
    VIDEO_CLOSE_IMAGE_NUM,
    VIDEO_UPLOAD_IMAGE_NUM,

    /* Video capture enhancement */
    VIDEO_PIN_BUFFER_NUM,
    VIDEO_UNPIN_BUFFER_NUM,

    /* save current process settings and osd settings as default for this device */
    SETTINGS_SAVE_AS_PRESET_NUM,
    /* reload default settings to overwrite current process settings and osd settings */
    SETTINGS_RELOAD_PRESET_NUM,

    // Video processing (NOT functional for DirectIOCTL)
    GET_VIDEO_CONNECTION_FORMAT_NUM,
    GET_VIDEO_PROCESS_SETTINGS_NUM,
    SET_VIDEO_PROCESS_SETTINGS_NUM,
    GET_VIDEO_OSD_SETTINGS_NUM,
    SET_VIDEO_OSD_SETTINGS_NUM,
    GET_VIDEO_OSD_IMAGE_NUM,
    SET_VIDEO_OSD_IMAGE_NUM,

    GET_BRIGHTNESS_NUM,
    SET_BRIGHTNESS_NUM,
    GET_CONTRAST_NUM,
    SET_CONTRAST_NUM,
    GET_HUE_NUM,
    SET_HUE_NUM,
    GET_SATURATION_NUM,
    SET_SATURATION_NUM,

    SET_SDI_ANC_TYPE,
    GET_SDI_ANC_PACKET,

    GET_V4L2_FRAME_SDI_ANC_PACKETS_NUM,

    GET_ENABLE_V4L2_FRAME_SDI_ANC_NUM,
    SET_ENABLE_V4L2_FRAME_SDI_ANC_NUM,

    VIDEO_ECO_CAPTURE_OPEN_NUM,
    VIDEO_ECO_CAPTURE_GET_SETTINGS_NUM,
    VIDEO_ECO_CAPTURE_SET_SETTINGS_NUM,
    VIDEO_ECO_CAPTURE_FRAME_NUM,
    VIDEO_ECO_CAPTURE_STATUS_NUM,
    VIDEO_ECO_CAPTURE_CLOSE_NUM
};

typedef struct _MWCAP_STREAM_INFO {
    int                 stream_id; /* >1 */
    pid_t               pid;    /* proc id */
    char                comm[MW_MAX_PROCESS_NAME_LEN]; /* executable name excluding path */
    bool                streaming; /* ONLY for os local function, not mwcap */
} MWCAP_STREAM_INFO;

typedef struct _MWCAP_STREAMS_INFO {
    int                     count;
    MWCAP_PTR               infos; /* (MWCAP_STREAM_INFO, infos) */
} MWCAP_STREAMS_INFO;

typedef struct _MWCAP_FIRMWARE_DATA {
    unsigned int        offset;
    unsigned int        size; /* data size */
    MWCAP_PTR           data; /* (unsigned char * data) */
} MWCAP_FIRMWARE_DATA;

typedef struct _MWCAP_INPUT_SOURCE_ARRAY {
    unsigned int        count; /* count of element, user set */
    MWCAP_PTR           data; /* (unsigned int * data) */
} MWCAP_INPUT_SOURCE_ARRAY;

typedef struct _MWCAP_EDID_DATA {
    int                 size;
    MWCAP_PTR           data; /* (unsigned char * data) */
} MWCAP_EDID_DATA;

typedef struct _MWCAP_HDMI_INFOFRAME_PACKET {
    unsigned int                id;
    HDMI_INFOFRAME_PACKET       pkt;
} MWCAP_HDMI_INFOFRAME_PACKET;

typedef struct _MWCAP_VIDEO_TIMING_PAR {
    int                                 count; // max: MWCAP_VIDEO_MAX_NUM_PREFERRED_TIMINGS
    MWCAP_PTR                           timings; /* (MWCAP_VIDEO_TIMING * timings) */
} MWCAP_VIDEO_TIMING_PAR;

typedef struct _MWCAP_VIDEO_CUSTOM_TIMING_PAR {
    int                                 count;
    MWCAP_PTR                           timings; /* (MWCAP_VIDEO_CUSTOM_TIMING * timings) */
} MWCAP_VIDEO_CUSTOM_TIMING_PAR;

typedef struct _MWCAP_VIDEO_CUSTOM_RESOLUTION_PAR {
    int                                 count;
    MWCAP_PTR                           resolutions; /* (MWCAP_SIZE * resolutions) */
} MWCAP_VIDEO_CUSTOM_RESOLUTION_PAR;

typedef struct _MWCAP_VIDEO_FRAME_INFO_PAR {
    int                                 iframe; // set
    MWCAP_VIDEO_FRAME_INFO              info;   // get
} MWCAP_VIDEO_FRAME_INFO_PAR;


#define MWCAP_IOCTL_GET_STREAMS_COUNT                      _IOR ('X', GET_STREAMS_COUNT_NUM, int)
#define MWCAP_IOCTL_GET_STREAMS_INFO                       _IOWR('X', GET_STREAMS_INFO_NUM, MWCAP_STREAMS_INFO)
#define MWCAP_IOCTL_GET_CTRL_STREAM_ID                     _IOR ('X', GET_CTRL_STREAM_ID_NUM, int)
#define MWCAP_IOCTL_SET_CTRL_STREAM_ID                     _IOW ('X', SET_CTRL_STREAM_ID_NUM, int)
#define MWCAP_IOCTL_GET_SELF_STREAM_ID                     _IOR ('X', GET_SELF_STREAM_ID_NUM, int)

#define MWCAP_IOCTL_GET_CHANNEL_INFO                       _IOR ('X', GET_CHANNEL_INFO_NUM, MWCAP_CHANNEL_INFO)
#define MWCAP_IOCTL_GET_FAMILY_INFO                        _IOR ('X', GET_FAMILY_INFO_NUM, MWCAP_PRO_CAPTURE_INFO)
#define MWCAP_IOCTL_GET_VIDEO_CAPS                         _IOR ('X', GET_VIDEO_CAPS_NUM, MWCAP_VIDEO_CAPS)
#define MWCAP_IOCTL_GET_AUDIO_CAPS                         _IOR ('X', GET_AUDIO_CAPS_NUM, MWCAP_AUDIO_CAPS)

// Firmware upgrade
#define MWCAP_IOCTL_GET_FIRMWARE_STORAGE                   _IOR ('X', GET_FIRMWARE_STORAGE_NUM, MWCAP_FIRMWARE_STORAGE)
#define MWCAP_IOCTL_SET_FIRMWARE_ERASE                     _IOW ('X', SET_FIRMWARE_ERASE_NUM, MWCAP_FIRMWARE_ERASE)
#define MWCAP_IOCTL_SET_FIRMWARE_DATA                      _IOW ('X', SET_FIRMWARE_DATA_NUM, MWCAP_FIRMWARE_DATA)
#define MWCAP_IOCTL_GET_FIRMWARE_DATA                      _IOWR('X', GET_FIRMWARE_DATA_NUM, MWCAP_FIRMWARE_DATA)

// Device misc controls
#define MWCAP_IOCTL_SET_LED_MODE                           _IOW ('X', SET_LED_MODE_NUM, unsigned int)
#define MWCAP_IOCTL_SET_POST_RECONFIG                      _IOW ('X', SET_POST_RECONFIG_NUM, unsigned int)
#define MWCAP_IOCTL_GET_CORE_TEMPERATURE                   _IOR ('X', GET_CORE_TEMPERATURE_NUM, unsigned int)

// Hardware timer
#define MWCAP_IOCTL_GET_TIME                               _IOR ('X', GET_TIME_NUM, long long)
#define MWCAP_IOCTL_SET_TIME                               _IOW ('X', SET_TIME_NUM, long long)
#define MWCAP_IOCTL_TIME_REGULATION                        _IOW ('X', TIME_REGULATION_NUM, long long)

#define MWCAP_IOCTL_TIMER_REGISTRATION                     _IOWR('X', TIMER_REGISTRATION_NUM, MWCAP_TIMER_REGISTRATION_S)
#define MWCAP_IOCTL_TIMER_DEREGISTRATION                   _IOW ('X', TIMER_DEREGISTRATION_NUM, MWCAP_PTR)
#define MWCAP_IOCTL_TIMER_EXPIRE_TIME                      _IOW ('X', TIMER_EXPIRE_TIME_NUM, MWCAP_TIMER_EXPIRE_TIME)

// Notifications
#define MWCAP_IOCTL_NOTIFY_REGISTRATION                    _IOWR('X', NOTIFY_REGISTRATION_NUM, MWCAP_NOTIFY_REGISTRATION_S)
#define MWCAP_IOCTL_NOTIFY_DEREGISTRATION                  _IOW ('X', NOTIFY_DEREGISTRATION_NUM, MWCAP_PTR)
#define MWCAP_IOCTL_NOTIFY_STATUS                          _IOWR('X', NOTIFY_STATUS_NUM, MWCAP_NOTIFY_STATUS)
#define MWCAP_IOCTL_NOTIFY_ENABLE                          _IOW ('X', NOTIFY_ENABLE_NUM, MWCAP_NOTIFY_ENABLE)

// Video frame information
#define MWCAP_IOCTL_GET_VIDEO_BUFFER_INFO                  _IOR ('X', GET_VIDEO_BUFFER_INFO_NUM, MWCAP_VIDEO_BUFFER_INFO)
#define MWCAP_IOCTL_GET_VIDEO_FRAME_INFO                   _IOWR('X', GET_VIDEO_FRAME_INFO_NUM, MWCAP_VIDEO_FRAME_INFO_PAR)

// Video capture
#define MWCAP_IOCTL_VIDEO_CAPTURE_OPEN                     _IOW ('X', VIDEO_CAPTURE_OPEN_NUM, MWCAP_VIDEO_CAPTURE_OPEN)
#define MWCAP_IOCTL_VIDEO_CAPTURE_FRAME                    _IOW ('X', VIDEO_CAPTURE_FRAME_NUM, MWCAP_VIDEO_CAPTURE_FRAME)
#define MWCAP_IOCTL_VIDEO_CAPTURE_STATUS                   _IOR ('X', VIDEO_CAPTURE_STATUS_NUM, MWCAP_VIDEO_CAPTURE_STATUS)
#define MWCAP_IOCTL_VIDEO_CAPTURE_CLOSE                    _IO  ('X', VIDEO_CAPTURE_CLOSE_NUM)

// Audio capture
#define MWCAP_IOCTL_AUDIO_CAPTURE_OPEN                     _IO  ('X', AUDIO_CAPTURE_OPEN_NUM)
#define MWCAP_IOCTL_AUDIO_CAPTURE_FRAME                    _IOR ('X', AUDIO_CAPTURE_FRAME_NUM, MWCAP_AUDIO_CAPTURE_FRAME)
#define MWCAP_IOCTL_AUDIO_CAPTURE_CLOSE                    _IO  ('X', AUDIO_CAPTURE_CLOSE_NUM)

// Input source
#define MWCAP_IOCTL_VIDEO_INPUT_SOURCE_COUNT               _IOR ('X', VIDEO_INPUT_SOURCE_COUNT_NUM, int)
#define MWCAP_IOCTL_VIDEO_INPUT_SOURCE_ARRAY               _IOWR('X', VIDEO_INPUT_SOURCE_ARRAY_NUM, MWCAP_INPUT_SOURCE_ARRAY)

#define MWCAP_IOCTL_AUDIO_INPUT_SOURCE_COUNT               _IOR ('X', AUDIO_INPUT_SOURCE_COUNT_NUM, int)
#define MWCAP_IOCTL_AUDIO_INPUT_SOURCE_ARRAY               _IOWR('X', AUDIO_INPUT_SOURCE_ARRAY_NUM, MWCAP_INPUT_SOURCE_ARRAY)

#define MWCAP_IOCTL_SET_INPUT_SOURCE_SCAN                  _IOW ('X', SET_INPUT_SOURCE_SCAN_NUM, bool)
#define MWCAP_IOCTL_GET_INPUT_SOURCE_SCAN                  _IOR ('X', GET_INPUT_SOURCE_SCAN_NUM, bool)
#define MWCAP_IOCTL_GET_INPUT_SOURCE_SCAN_STATE            _IOR ('X', GET_INPUT_SOURCE_SCAN_STATE_NUM, bool)
#define MWCAP_IOCTL_SET_AV_INPUT_SOURCE_LINK               _IOW ('X', SET_AV_INPUT_SOURCE_LINK_NUM, bool)
#define MWCAP_IOCTL_GET_AV_INPUT_SOURCE_LINK               _IOR ('X', GET_AV_INPUT_SOURCE_LINK_NUM, bool)
#define MWCAP_IOCTL_SET_VIDEO_INPUT_SOURCE                 _IOW ('X', SET_VIDEO_INPUT_SOURCE_NUM, unsigned int)
#define MWCAP_IOCTL_GET_VIDEO_INPUT_SOURCE                 _IOR ('X', GET_VIDEO_INPUT_SOURCE_NUM, unsigned int)
#define MWCAP_IOCTL_SET_AUDIO_INPUT_SOURCE                 _IOW ('X', SET_AUDIO_INPUT_SOURCE_NUM, unsigned int)
#define MWCAP_IOCTL_GET_AUDIO_INPUT_SOURCE                 _IOR ('X', GET_AUDIO_INPUT_SOURCE_NUM, unsigned int)

// EDID
#define MWCAP_IOCTL_GET_EDID_DATA                          _IOWR('X', GET_EDID_DATA_NUM, MWCAP_EDID_DATA)
#define MWCAP_IOCTL_SET_EDID_DATA                          _IOW ('X', SET_EDID_DATA_NUM, MWCAP_EDID_DATA)

// Signal status
#define MWCAP_IOCTL_GET_INPUT_SPECIFIC_STATUS              _IOR ('X', GET_INPUT_SPECIFIC_STATUS_NUM, MWCAP_INPUT_SPECIFIC_STATUS)
#define MWCAP_IOCTL_GET_VIDEO_SIGNAL_STATUS                _IOR ('X', GET_VIDEO_SIGNAL_STATUS_NUM, MWCAP_VIDEO_SIGNAL_STATUS)
#define MWCAP_IOCTL_GET_AUDIO_SIGNAL_STATUS                _IOR ('X', GET_AUDIO_SIGNAL_STATUS_NUM, MWCAP_AUDIO_SIGNAL_STATUS)

// HDMI status
#define MWCAP_IOCTL_GET_HDMI_INFOFRAME_VALID               _IOR ('X', GET_HDMI_INFOFRAME_VALID_NUM, unsigned int)
#define MWCAP_IOCTL_GET_HDMI_INFOFRAME_PACKET              _IOWR('X', GET_HDMI_INFOFRAME_PACKET_NUM, MWCAP_HDMI_INFOFRAME_PACKET)

// Video processing
#define MWCAP_IOCTL_GET_VIDEO_INPUT_ASPECT_RATIO           _IOR ('X', GET_VIDEO_INPUT_ASPECT_RATIO_NUM, MWCAP_VIDEO_ASPECT_RATIO)
#define MWCAP_IOCTL_SET_VIDEO_INPUT_ASPECT_RATIO           _IOW ('X', SET_VIDEO_INPUT_ASPECT_RATIO_NUM, MWCAP_VIDEO_ASPECT_RATIO)
#define MWCAP_IOCTL_GET_VIDEO_INPUT_COLOR_FORMAT           _IOR ('X', GET_VIDEO_INPUT_COLOR_FORMAT_NUM, MWCAP_VIDEO_COLOR_FORMAT)
#define MWCAP_IOCTL_SET_VIDEO_INPUT_COLOR_FORMAT           _IOW ('X', SET_VIDEO_INPUT_COLOR_FORMAT_NUM, MWCAP_VIDEO_COLOR_FORMAT)
#define MWCAP_IOCTL_GET_VIDEO_INPUT_QUANTIZATION_RANGE     _IOR ('X', GET_VIDEO_INPUT_QUANTIZATION_RANGE_NUM, MWCAP_VIDEO_QUANTIZATION_RANGE)
#define MWCAP_IOCTL_SET_VIDEO_INPUT_QUANTIZATION_RANGE     _IOW ('X', SET_VIDEO_INPUT_QUANTIZATION_RANGE_NUM, MWCAP_VIDEO_QUANTIZATION_RANGE)

// Image upload, can be use as source image or OSD image
#define MWCAP_IOCTL_VIDEO_CREATE_IMAGE                     _IOWR('X', VIDEO_CREATE_IMAGE_NUM, MWCAP_VIDEO_CREATE_IMAGE)
#define MWCAP_IOCTL_VIDEO_OPEN_IMAGE                       _IOWR('X', VIDEO_OPEN_IMAGE_NUM, MWCAP_VIDEO_IMAGE_REF)
#define MWCAP_IOCTL_VIDEO_CLOSE_IMAGE                      _IOWR('X', VIDEO_CLOSE_IMAGE_NUM, MWCAP_VIDEO_IMAGE_REF)
#define MWCAP_IOCTL_VIDEO_UPLOAD_IMAGE                     _IOW ('X', VIDEO_UPLOAD_IMAGE_NUM, MWCAP_VIDEO_UPLOAD_IMAGE)

// VGA/Component timings
#define MWCAP_IOCTL_GET_VIDEO_AUTO_H_ALIGN                 _IOR ('X', GET_VIDEO_AUTO_H_ALIGN_NUM, BOOLEAN)
#define MWCAP_IOCTL_SET_VIDEO_AUTO_H_ALIGN                 _IOW ('X', SET_VIDEO_AUTO_H_ALIGN_NUM, BOOLEAN)
    /* (0-31, 255 triger auto adjust once) */
#define MWCAP_IOCTL_GET_VIDEO_SAMPLING_PHASE               _IOR ('X', GET_VIDEO_SAMPLING_PHASE_NUM, BYTE)
#define MWCAP_IOCTL_SET_VIDEO_SAMPLING_PHASE               _IOW ('X', SET_VIDEO_SAMPLING_PHASE_NUM, BYTE)
#define MWCAP_IOCTL_GET_VIDEO_SAMPLING_PHASE_AUTO          _IOR ('X', GET_VIDEO_SAMPLING_PHASE_AUTO_NUM, BOOLEAN)
#define MWCAP_IOCTL_SET_VIDEO_SAMPLING_PHASE_AUTO          _IOW ('X', SET_VIDEO_SAMPLING_PHASE_AUTO_NUM, BOOLEAN)
#define MWCAP_IOCTL_SET_VIDEO_TIMING                       _IOW ('X', SET_VIDEO_TIMING_NUM, MWCAP_VIDEO_TIMING)
#define MWCAP_IOCTL_GET_VIDEO_PREFERRED_TIMING_ARRAY       _IOWR('X', GET_VIDEO_PREFERRED_TIMING_ARRAY_NUM, MWCAP_VIDEO_TIMING_PAR)
#define MWCAP_IOCTL_SET_VIDEO_CUSTOM_TIMING                _IOW ('X', SET_VIDEO_CUSTOM_TIMING_NUM, MWCAP_VIDEO_CUSTOM_TIMING)
#define MWCAP_IOCTL_GET_VIDEO_CUSTOM_TIMING_COUNT          _IOR ('X', GET_VIDEO_CUSTOM_TIMING_COUNT_NUM, DWORD)
#define MWCAP_IOCTL_GET_VIDEO_CUSTOM_TIMING_ARRAY          _IOWR('X', GET_VIDEO_CUSTOM_TIMING_ARRAY_NUM, MWCAP_VIDEO_CUSTOM_TIMING_PAR)
#define MWCAP_IOCTL_SET_VIDEO_CUSTOM_TIMING_ARRAY          _IOW ('X', SET_VIDEO_CUSTOM_TIMING_ARRAY_NUM, MWCAP_VIDEO_CUSTOM_TIMING_PAR)
#define MWCAP_IOCTL_GET_VIDEO_CUSTOM_RESOLUTION_COUNT      _IOR ('X', GET_VIDEO_CUSTOM_RESOLUTION_COUNT_NUM, DWORD)
#define MWCAP_IOCTL_GET_VIDEO_CUSTOM_RESOLUTION_ARRAY      _IOWR('X', GET_VIDEO_CUSTOM_RESOLUTION_ARRAY_NUM, MWCAP_VIDEO_CUSTOM_RESOLUTION_PAR)
#define MWCAP_IOCTL_SET_VIDEO_CUSTOM_RESOLUTION_ARRAY      _IOW ('X', SET_VIDEO_CUSTOM_RESOLUTION_ARRAY_NUM, MWCAP_VIDEO_CUSTOM_RESOLUTION_PAR)

/* Video capture enhancement */
#define MWCAP_IOCTL_VIDEO_PIN_BUFFER                       _IOW ('X', VIDEO_PIN_BUFFER_NUM, MWCAP_VIDEO_PIN_BUFFER)
#define MWCAP_IOCTL_VIDEO_UNPIN_BUFFER                     _IOW ('X', VIDEO_UNPIN_BUFFER_NUM, MWCAP_PTR)

/* save current process settings and osd settings as default for this device */
#define MWCAP_IOCTL_SETTINGS_SAVE_AS_PRESET                _IO  ('X', SETTINGS_SAVE_AS_PRESET_NUM)
/* reload default settings to overwrite current process settings and osd settings */
#define MWCAP_IOCTL_SETTINGS_RELOAD_PRESET                 _IO  ('X', SETTINGS_RELOAD_PRESET_NUM)

// Video processing (NOT functional for mwcap stream)
#define MWCAP_IOCTL_GET_VIDEO_CONNECTION_FORMAT            _IOR ('X', GET_VIDEO_CONNECTION_FORMAT_NUM, MWCAP_VIDEO_CONNECTION_FORMAT)
#define MWCAP_IOCTL_GET_VIDEO_PROCESS_SETTINGS             _IOR ('X', GET_VIDEO_PROCESS_SETTINGS_NUM, MWCAP_VIDEO_PROCESS_SETTINGS)
#define MWCAP_IOCTL_SET_VIDEO_PROCESS_SETTINGS             _IOW ('X', SET_VIDEO_PROCESS_SETTINGS_NUM, MWCAP_VIDEO_PROCESS_SETTINGS)
#define MWCAP_IOCTL_GET_VIDEO_OSD_SETTINGS                 _IOR ('X', GET_VIDEO_OSD_SETTINGS_NUM, MWCAP_VIDEO_OSD_SETTINGS)
#define MWCAP_IOCTL_SET_VIDEO_OSD_SETTINGS                 _IOW ('X', SET_VIDEO_OSD_SETTINGS_NUM, MWCAP_VIDEO_OSD_SETTINGS)
#define MWCAP_IOCTL_GET_VIDEO_OSD_IMAGE                    _IOR ('X', GET_VIDEO_OSD_IMAGE_NUM, MWCAP_VIDEO_OSD_IMAGE)
#define MWCAP_IOCTL_SET_VIDEO_OSD_IMAGE                    _IOW ('X', SET_VIDEO_OSD_IMAGE_NUM, MWCAP_VIDEO_OSD_IMAGE)

// Video AMP
#define MWCAP_IOCTL_GET_BRIGHTNESS                         _IOR ('X', GET_BRIGHTNESS_NUM, int)
#define MWCAP_IOCTL_SET_BRIGHTNESS                         _IOW ('X', SET_BRIGHTNESS_NUM, int)
#define MWCAP_IOCTL_GET_CONTRAST                           _IOR ('X', GET_CONTRAST_NUM, int)
#define MWCAP_IOCTL_SET_CONTRAST                           _IOW ('X', SET_CONTRAST_NUM, int)
#define MWCAP_IOCTL_GET_HUE                                _IOR ('X', GET_HUE_NUM, int)
#define MWCAP_IOCTL_SET_HUE                                _IOW ('X', SET_HUE_NUM, int)
#define MWCAP_IOCTL_GET_SATURATION                         _IOR ('X', GET_SATURATION_NUM, int)
#define MWCAP_IOCTL_SET_SATURATION                         _IOW ('X', SET_SATURATION_NUM, int)

// SDI ANC Packet
#define MWCAP_IOCTL_SET_SDI_ANC_TYPE                       _IOW ('X', SET_SDI_ANC_TYPE, MWCAP_SDI_ANC_TYPE)
#define MWCAP_IOCTL_GET_SDI_ANC_PACKET                     _IOR ('X', GET_SDI_ANC_PACKET, MWCAP_SDI_ANC_PACKET)


#define MWCAP_IOCTL_VIDEO_ECO_CAPTURE_OPEN                 _IOW ('X', VIDEO_ECO_CAPTURE_OPEN_NUM, MWCAP_VIDEO_ECO_CAPTURE_OPEN)
#define MWCAP_IOCTL_VIDEO_ECO_CAPTURE_GET_SETTINGS         _IOR ('X', VIDEO_ECO_CAPTURE_GET_SETTINGS_NUM, MWCAP_VIDEO_ECO_CAPTURE_SETTINGS)
#define MWCAP_IOCTL_VIDEO_ECO_CAPTURE_SET_SETTINGS         _IOW ('X', VIDEO_ECO_CAPTURE_SET_SETTINGS_NUM, MWCAP_VIDEO_ECO_CAPTURE_SETTINGS)
#define MWCAP_IOCTL_VIDEO_ECO_CAPTURE_FRAME                _IOW ('X', VIDEO_ECO_CAPTURE_FRAME_NUM, MWCAP_VIDEO_ECO_CAPTURE_FRAME)
#define MWCAP_IOCTL_VIDEO_ECO_CAPTURE_STATUS               _IOR ('X', VIDEO_ECO_CAPTURE_STATUS_NUM, MWCAP_VIDEO_ECO_CAPTURE_STATUS)
#define MWCAP_IOCTL_VIDEO_ECO_CAPTURE_CLOSE                _IO  ('X', VIDEO_ECO_CAPTURE_CLOSE_NUM)

#pragma pack(pop)

#endif /* __MW_LINUX__ */
