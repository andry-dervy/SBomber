cmake_minimum_required(VERSION 3.14)

project(SBomber LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    main.cpp

    include/DestroyableGroundObject.h
    include/DynamicObject.h
    include/GameObject.h

    include/House.h
    src/House.cpp

    include/LevelGUI.h
    src/LevelGUI.cpp

    include/MyTools.h
    src/MyTools.cpp

    include/Plane.h
    src/Plane.cpp

    include/SBomber.h
    src/SBomber.cpp

    include/Tank.h
    src/Tank.cpp

    include/Bomb.h
    src/Bomb.cpp

    include/Ground.h
    src/Ground.cpp

    include/ScreenSingleton.h
    src/ScreenSingleton.cpp

    include/Crater.h
    src/Crater.cpp

    include/enums/ConsoleColors.h
    include/enums/CraterSize.h

    include/Tower.h
    src/Tower.cpp
)

add_executable(SBomber
  ${SOURCES}
)
