#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Camera.h"

/// The main configuration for the whole game. It defines parameters than can be
/// useful to any part of the game. It also includes parameters specific to some
/// rendering or whatever.
class Configuration {
      public:
        /// The base constructor
        Configuration();
        /// The base destructor
        ~Configuration();

        /// The width of the window when created
        int windowWidth = 1000;
        /// The height of the window when created
        int windowHeight = 600;
        /// Wheter or not the window is full screen
        bool isFullScreen = false;
        /// The amount of chunk visible on one side. (Maybe more are loaded on
        /// the memory).
        ///
        /// For example, a render distance of 3 looks like:
        ///```
        ///             r r r r r r r
        ///             r r r r r r r
        ///             r r r r r r r
        ///             r r r x r r r
        ///             r r r r r r r
        ///             r r r r r r r
        ///             r r r r r r r
        /// ```
        /// With r being rendered (at least loaded on the gpu, maybe not
        /// actually rendered because of frustum culling), and x the chunk where
        /// the player stands.
        int renderDistance = 16;
        /// Th field of view which is used to compute the camera projection
        /// matrix.
        int fov = 90;
        /// Whether the mouse is free from the window. True: the mouse can move
        /// out of the window, false : the cursor is hidden, can't move out of
        /// the window and controls the view direction.
        bool mouseFree = false;
        /// Whether to show th eimgui based debug infos window
        bool showDebugInfos = false;
        /// How much the cam moves with one keystroke (some arbitrary unit used
        /// there).
        int camSpeedFactor = 5;
        /// Whether we are flying (to know which physics profile to follow (not
        /// yet implemented)).
        bool isFlying = true;
        /// Whether or not to check collisions
        bool checkCollisions = true;
        /// Getter of #checkCollisions (used in some callbacks)
        bool getCheckCollisions();
        /// Setter of #checkCollisions (used in some callbacks)
        bool setCheckCollisions(bool val);

      private:
};

#endif
