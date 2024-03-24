
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
#find_package(SFML )
set_source_files_properties(tags PROPERTIES GENERATED true)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
add_custom_command(OUTPUT tags
        COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(${PROJECT_NAME} main.cpp Model/AnyObject.cpp
        Model/Mesh.cpp
        Model/Vertex.cpp
        Model/Primitives.cpp
        Model/World.cpp
        Model/Camera.cpp
        Model/Renderer.cpp
        Model/Screen.cpp
        Model/Utils.cpp
        Model/Object.cpp
        Model/Model.cpp
        View/View.cpp
        Controller/Controller.cpp
        Application.cpp
        tags
)
target_link_libraries(${PROJECT_NAME} sfml-graphics )

#set(${CMAKE_PREFIX_PATH} ~/)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)

#target_link_libraries(${PROJECT_NAME} Qt5::Core)

# set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "main.out")
