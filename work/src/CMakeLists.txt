#
# COMP 308
# If you add new source files, you must list them here
#

# TODO list your header files (.hpp) here
SET(headers
	"comp308.hpp"
	"imageLoader.hpp"
	"shaderLoader.hpp"
	"geometry.hpp"
	"geometries.hpp"
	"lights.hpp"
	"particleSystem.hpp"
	"particleSystemBasic.hpp"
	"particleSystemSwarm.hpp"
	"particleConstraint.hpp"
	"particleConstraintPin.hpp"
	"particleConstraintDistance.hpp"
	"particleConstraintAngle.hpp"
	"particleConstraintPlane.hpp"
	"lsystemtree.hpp"
)


# TODO list your source files (.cpp) here
SET(sources
	"main.cpp"
	"geometry.cpp"
	"geometries.cpp"
	"lights.cpp"	
	"particleSystem.cpp"
	"lsystemtree.cpp"
)

# Add executable target and link libraries
# You do not need to touch this
add_executable(${COMP308_ASSIGNMENT} ${headers} ${sources})
target_link_libraries(${COMP308_ASSIGNMENT} PRIVATE GLUT::GLUT glew)
target_link_libraries(${COMP308_ASSIGNMENT} PRIVATE stb)