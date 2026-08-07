/// Configuration struct for the whole app
const Configuration = struct {
    window_width: u16 = 1000,
    window_height: u16 = 600,
    is_full_screen: bool = false,
    render_distance: u16 = 8,
    fov: u8 = 90,
    mouse_free: bool = false,
    show_debug_infos: bool = false,
    camSpeeFactor: u8 = 5,
    is_flying: bool = true,
    check_collisions: bool = true,
};
