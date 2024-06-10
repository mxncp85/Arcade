#pragma once

namespace arc
{
    enum Event
    {
        // Game event
        EventUp,
        EventDown,
        EventLeft,
        EventRight,
        EventAction,

        // Core event
        EventPreviousGame,
        EventNextGame,
        EventPreviousGraphical,
        EventNextGraphical,
        EventRestart,
        EventExit
    };

    enum Color
    {
        ColorBlack,
        ColorRed,
        ColorGreen,
        ColorYellow,
        ColorBlue,
        ColorMagenta,
        ColorCyan,
        ColorWhite
    };

    enum Orientation
    {
        Orientation0,
        Orientation90,
        Orientation180,
        Orientation270,
        OrientationMirror0,
        OrientationMirror90,
        OrientationMirror180,
        OrientationMirror270,
    };
}
