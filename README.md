`GOSDClock` (Gtk OSD Clock) is a utility program which shows the current time at any corner of the display.

This program is originally inspired from [kwin-effects-osdclock](https://store.kde.org/p/1253662).

![Preview](assets/image.jpg)

## Why
- Might be useful for setups with panel set to autohide
- Might be used by people willing to maximize their OCD and ADHD

## Features
- Written in Gtk3, works on both X11 and Wayland
- Compiles into two executables: `gosdclock` (main executable) and `gosdclock_settings` (settings program)
- Automatically hides when hovered on it
- Can be further styled with `gtk.css`

## Compiling and Testing
```
git clone https://github.com/mehedirm6244/gosdclock
cd gosdclock
mkdir build; cd build
cmake ..
cmake --build .
```

### Run `gosdclock` executable
```
./gosdclock
```


## Customization
### Using `gosdclock_settings` executable
Run `gosdclock_settings` :
```
./gosdclock_settings
```

#### Available settings:
- Font Style (font family and size)
- OCD Position (Top left, Top Right, Bottom Left, Bottom Right)
- Time Format (12H, 12H with seconds, 24H, 24H with seconds)
- X and Y axis offsets

### Using `gtk.css`
`gosdclock` window has the class `gosdclock` and `gosdclock_hidden` (activated when hovered) to provide necessary CSS support.

Example `gtk.css`
```
.gosdclock label {
	text-shadow: 0 0 3px black;
	transition: 0.1s;
	color: #eee;
	opacity: 0.9;
}

.gosdclock_hidden label {
	opacity: 0;
}
```

## Known Bugs
- Flickering when hovered (expected behavior). Need X11 and Wayland-specific fixes
- Disappears when fullscreen is activated for any window. Requires X11 and wayland-specific fixes
