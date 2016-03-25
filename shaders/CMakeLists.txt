set(SHADER_SOURCE
	# Shaders go here
	"${ASSIGNMENT_SHADER_ROOT}/ball.vs.glsl"
	"${ASSIGNMENT_SHADER_ROOT}/ball.fs.glsl"
	"${ASSIGNMENT_SHADER_ROOT}/grid.vs.glsl"
	"${ASSIGNMENT_SHADER_ROOT}/grid.fs.glsl"
)

# Tell the compiler/IDE to treat the shader files as headers and ignore them
# during compile
set_source_files_properties(${SHADER_SOURCE} PROPERTIES HEADER_FILE_ONLY TRUE)


# I'll explain below what this is for.
set(SHADER_INCLUDE "${ASSIGNMENT_GENERATED_ROOT}/ShaderPaths.hpp")

# Let me explain what the generated folder is and why we need it. Since CMake generates
# the executable inside the build directory (and even then there is no unified path for it)
# we need a way of somehow getting the path for our shaders so the executable can find and
# compile them. The way I found (which for the record isn't necessarily the most elegant way)
# is to generate a header that contains the absolute path to the shader directory. That way, 
# we can always ensure that the executable can find them. 
#
# If you're curious as to what the elegant way is, it involves copying the shader folder into
# the folder where the executable lands. The thing is that I haven't found a cross-platform way
# of solving this, since it works on Unix systems but breaks on Windows (I am looking into it though
# and will be posting a better version once I find it).
# 
# Anyway, so we basically generate a header with the absolute path to our shader directory, dump it in
# the generated folder, and then we just add it to our list.
configure_file("${ASSIGNMENT_SHADER_ROOT}/ShaderPaths.hpp.in" ${SHADER_INCLUDE})

set(SHADER_LIST ${SHADER_SOURCE} ${SHADER_INCLUDE} PARENT_SCOPE)
