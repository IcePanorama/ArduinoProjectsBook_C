#include "pwm/waveform_generation_mode.h"

#include <avr/io.h>

#define WAVEFORM_GENERATION_MODE_BIT_0 (WGM00)
#define WAVEFORM_GENERATION_MODE_BIT_1 (WGM01)

#define WAVEFORM_GENERATION_MODE_BIT_2 (WGM02)

bool
wgm_is_valid_waveform_gen_mode (WaveformGenerationMode_t w)
{
  switch (w)
    {
    case WGM_MODE_0:
    case WGM_MODE_1:
    case WGM_MODE_2:
    case WGM_MODE_3:
    case WGM_MODE_5:
    case WGM_MODE_7:
      return true;
    default:
      return false;
    }
}

void
wgm_set_waveform_gen_mode (PMWTimerCntr_t *pmw, WaveformGenerationMode_t w)
{
  switch (w)
    {
    case WGM_MODE_0:
      pmw->control_register_a &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_1));
      pmw->control_register_b &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_2));
      break;
    case WGM_MODE_1:
      pmw->control_register_a |= (1 << (WAVEFORM_GENERATION_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_1));
      pmw->control_register_b &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_2));
      break;
    case WGM_MODE_2:
      pmw->control_register_a &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_0));
      pmw->control_register_a |= (1 << (WAVEFORM_GENERATION_MODE_BIT_1));
      pmw->control_register_b &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_2));
      break;
    case WGM_MODE_3:
      pmw->control_register_a |= (1 << (WAVEFORM_GENERATION_MODE_BIT_0));
      pmw->control_register_a |= (1 << (WAVEFORM_GENERATION_MODE_BIT_1));
      pmw->control_register_b &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_2));
      break;
    case WGM_MODE_5:
      pmw->control_register_a |= (1 << (WAVEFORM_GENERATION_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_1));
      pmw->control_register_b |= (1 << (WAVEFORM_GENERATION_MODE_BIT_2));
      break;
    case WGM_MODE_7:
      pmw->control_register_a |= (1 << (WAVEFORM_GENERATION_MODE_BIT_0));
      pmw->control_register_a |= (1 << (WAVEFORM_GENERATION_MODE_BIT_1));
      pmw->control_register_b |= (1 << (WAVEFORM_GENERATION_MODE_BIT_2));
      break;
    }
}
