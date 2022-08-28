#ifndef __ATOMS_H__
#define __ATOMS_H__

// These are the same atoms used by dwm as of the
// initial writing of this code
typedef enum
{
    utf8_string,

    wm_protocols,
    wm_delete_window,
    wm_state,
    wm_take_focus,

    // Used to specify the start of net_wm
    // atoms, do not move it
    net_start,
    
    net_active_window,
    net_wm_supported,
    net_wm_name,
    net_wm_state,
    net_supporting_wm_check,
    net_wm_state_fullscreen,
    net_wm_window_type,
    net_wm_window_type_dialog,
    net_client_list,

    atom_no_id
} atom_id_t;

extern const char *atom_str_identifiers[atom_no_id];

static inline const char* 
atom_to_str(atom_id_t id)
{
    return id < atom_no_id && id > 0 ?
        atom_str_identifiers[id] : "UNKNOWN_ATOM";
}

#endif