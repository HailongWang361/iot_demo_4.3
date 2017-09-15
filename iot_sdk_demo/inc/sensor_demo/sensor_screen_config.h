#ifdef CONFIG_INCLUDE_HEADER
#include "gnss_demo.h"
#include "bt_audio.h"
#endif

#include "fota_demo.h"
//void show_fota_screen(void);
//void fota_event_handler(message_id_enum event_id, int32_t param1, void* param2);


#ifdef CONFIG_INCLUD_BODY
// add your screen here
{show_fota_screen, fota_event_handler, "OTA demo"},
{show_bt_audio_screen, bt_audio_event_handler, "BT Audio"},

#endif

#ifdef CONFIG_MESSAGE_ID_ENUM
// define your own message bellowing here
//GNSS message
MESSAGE_ID_GNSS_POSITION,
MESSAGE_ID_GNSS_NMEA,
MESSAGE_ID_BT_AUDIO,
MESSAGE_ID_WATCH_FACE,

#endif
