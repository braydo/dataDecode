#include <stdio.h>
#include <stdint.h>
#include "jlabdec.h"
#include "mpd2dec.h"

void
mpd2DataDecode(uint32_t data)
{
  static uint32_t type_last = 15;	/* initialize to type FILLER WORD */
  static uint32_t time_last = 0;
  static int new_type = 0;
  int type_current = 0;
  static int apv_data_word = 0;
  generic_data_word_t gword;

  gword.raw = data;

  if(gword.bf.data_type_defining) /* data type defining word */
    {
      new_type = 1;
      type_current = gword.bf.data_type_tag;
    }
  else
    {
      new_type = 0;
      type_current = type_last;
    }

  switch( type_current )
    {
    case 0:		/* BLOCK HEADER */
      {
	block_header_t d; d.raw = data;

	printf("%8X - BLOCK HEADER - slot = %d  modID = %d   n_evts = %d   n_blk = %d\n",
	       d.raw,
	       d.bf.slot_number,
	       d.bf.module_ID,
	       d.bf.number_of_events_in_block,
	       d.bf.event_block_number);

	break;
      }

    case 1:		/* BLOCK TRAILER */
      {
	block_trailer_t d; d.raw = data;

	printf("%8X - BLOCK TRAILER - slot = %d   n_words = %d\n",
		     d.raw,
		     d.bf.slot_number,
		     d.bf.words_in_block);
	break;
      }

    case 2:		/* EVENT HEADER */
      {
	mpd2_event_header_t d; d.raw = data;

	printf("%8X - EVENT HEADER 1 - trig num = %d\n",
	       d.raw,
	       d.bf.trigger_number);
	break;
      }

    case 3:		/* TRIGGER TIME */
      {
	if( new_type )
	  {
	    mpd2_trigger_time_1_t d; d.raw = data;

	    printf("%8X - TRIGGER TIME 1 - time = %08x\n",
		   d.raw,
		   d.bf.trigger_time_h);

	    time_last = 1;
	  }
	else
	  {
	    mpd2_trigger_time_2_t d; d.raw = data;
	    if( time_last == 1 )
	      {
		printf("%8X - TRIGGER TIME 2 - time = %08x\n",
		       d.raw,
		       d.bf.trigger_time_l);
	      }
	    else
	      printf("%8X - TRIGGER TIME - (ERROR)\n", data);

	    time_last = 0;
	  }
	break;
      }

    case 5:		/* APV Frame */
      {
	if( new_type )
	  {
	    mpd2_apv_data_1_t d; d.raw = data;

	    printf("%8X - APVID = %2d, TSAMPLE = %d, FRAMECNT = %3d, APVHEADER = %3X\n",
		   d.raw,
		   d.bf.apv_id,
		   d.bf.sample_cnt,
		   d.bf.frame_cnt,
		   d.bf.apv_header);

	    apv_data_word = 1;
	  }
	else
	  {
      int s0,s1;
		  mpd2_apv_data_2_t d; d.raw = data;
      s0 = (d.bf.apv_sample0 & 0x1000) ? (d.bf.apv_sample0 | 0xFFFFF000) : d.bf.apv_sample0;
      s1 = (d.bf.apv_sample1 & 0x1000) ? (d.bf.apv_sample1 | 0xFFFFF000) : d.bf.apv_sample0;
		  printf("%8X - CH%3d=%5d CH%3d=%5d\n",
			 d.raw,
			 (apv_data_word-1)*2,
			 s0,
			 (apv_data_word-1)*2+1,
			 s1);

		  apv_data_word++;
		}
	break;
	      }

    case 14:		/* DATA NOT VALID (no data available) */
      {
	data_not_valid_t d; d.raw = data;

	printf("%8X - DATA NOT VALID = %d\n",
	       d.raw,
	       d.bf.data_type_tag);
	break;
      }

    case 15:		/* FILLER WORD */
      {
	filler_word_t d; d.raw = data;

	printf("%8X - FILLER WORD = %d\n",
	       d.raw,
	       d.bf.data_type_tag);
	break;
      }

    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    default:
      {
	printf("%8X - UNDEFINED TYPE = %d\n",
	       gword.raw,
	       gword.bf.data_type_tag);
	break;
      }

    }

  type_last = type_current;	/* save type of current data word */

}
