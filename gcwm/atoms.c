#include "atoms.h"

const char 
*atom_str_identifiers[atom_no_id] =
{
    [utf8_string]               = (char *)"UTF8_STRING",
    [wm_protocols]              = (char *)"WM_PROTOCOLS",
    [wm_delete_window]          = (char *)"WM_DELETE_WINDOW",
    [wm_state]                  = (char *)"WM_STATE",
    [wm_take_focus]             = (char *)"WM_TAKE_FOCUS",
    [net_active_window]         = (char *)"_NET_ACTIVE_WINDOW",
    [net_wm_supported]          = (char *)"_NET_SUPPORTED",
    [net_wm_name]               = (char *)"_NET_WM_NAME",
    [net_wm_state]              = (char *)"_NET_WM_STATE",
    [net_supporting_wm_check]   = (char *)"_NET_SUPPORTING_WM_CHECK",
    [net_wm_state_fullscreen]   = (char *)"_NET_WM_STATE_FULLSCREEN",
    [net_wm_window_type]        = (char *)"_NET_WM_WINDOW_TYPE",
    [net_wm_window_type_dialog] = (char *)"_NET_WM_WINDOW_TYPE_DIALOG",
    [net_client_list]           = (char *)"_NET_CLIENT_LIST"
};