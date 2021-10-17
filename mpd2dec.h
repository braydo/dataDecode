#ifndef __MPD2DEC__
#define __MPD2DEC__
#include <stdint.h>

/* 2: EVENT HEADER */
typedef struct
{
  uint32_t trigger_number:20;
  uint32_t undef:7;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} mpd2_event_header;

typedef union
{
  uint32_t raw;
  mpd2_event_header bf;
} mpd2_event_header_t;

/* 3: TRIGGER TIME */
typedef struct
{
  uint32_t trigger_time_h:24;
  uint32_t undef:3;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} mpd2_trigger_time_1;

typedef union
{
  uint32_t raw;
  mpd2_trigger_time_1 bf;
} mpd2_trigger_time_1_t;

typedef struct
{
  uint32_t trigger_time_l:24;
  uint32_t undef:3;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} mpd2_trigger_time_2;

typedef union
{
  uint32_t raw;
  mpd2_trigger_time_2 bf;
} mpd2_trigger_time_2_t;

/* 4: APV Data */
typedef struct
{
  uint32_t apv_header:12;
  uint32_t frame_cnt:8;
  uint32_t sample_cnt:3;
  uint32_t apv_id:4;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} mpd2_apv_data_1;

typedef union
{
  uint32_t raw;
  mpd2_apv_data_1 bf;
} mpd2_apv_data_1_t;

typedef struct
{
  uint32_t apv_sample0:13;
  uint32_t apv_sample1:13;
  uint32_t undef:5;
  uint32_t data_type_defining:1;
} mpd2_apv_data_2;

typedef union
{
  uint32_t raw;
  mpd2_apv_data_2 bf;
} mpd2_apv_data_2_t;

void  mpd2DataDecode(uint32_t data);


#endif /* __SSPAPVDEC__ */
