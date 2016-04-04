# ifndef SHADER_HPP
# define SHADER_HPP

# include <string>
# include <memory>
# include <unordered_map>

class ShaderProgram{
public:
  /**
   * Null shader program constructor. Perfectly valid shader program, but does
   * nothing.
   */
  ShaderProgram();

  /**
   * Shader program constructor from GLSL shader files. In the context of this
   * course, we consider only the vertex shader and the fragment shader.
   * However, there exist other programmable stage in the pipeline (see
   * www.opengl.org/sdk/docs/man/html/glCreateShader.xhtml).
   *
   * If the shaders are invalid or describe an invalid program, this is
   * initialized to the null shader program.
   *
   * @param vertex_file_path Path to the vertex shader file
   * @param fragment_file_path Path to the fragment shader file.
   */
  ShaderProgram(const std::string& vertex_file_path, const std::string& fragment_file_path );

  ~ShaderProgram();

  /**
   * Load shaders into this shader program. If the shaders are valid
   * (compilation stage) and they describe a valid program (linking stage),
   * this shader program would be new valid. Otherwise, this remains unchanged.
   *
   * @param vertex_file_path Path to the vertex shader file
   * @param fragment_file_path Path to the fragment shader file.
   */
  void load(const std::string& vertex_file_path, const std::string& fragment_file_path );

  /** @brief Reload the shader sources
   *
   *
   *
   */
  void reload();

  /**
   * Bind this program to the GPU. This is necessary to render objects or to
   * send uniforms/attributes values.
   */
  void bind();

  /**
   * Unbind any program.
   */
  static void unbind();

  /**
   * Return the location of a uniform thanks to its name. We use the locations
   * obtained by an introspection done after the linking stage. As such, we do
   * not request the GPU to answer this request: we have the answer on the CPU,
   * which would be faster.
   * @name The uniform name, as it appear in the shader sources (be aware of the
   * naming convention for blocks and arrays, see
   * https://www.opengl.org/wiki/Program_Introspection#Naming)
   * @return The uniform location, -1 if there is no uniform with such name in this program
   */
  int getUniformLocation( const std::string& name ) const;

  /**
   * Return the location of an attribute (a program input seen in the vertex
   * shader in our case).We use the locations obtained by an introspection done
   * after the linking stage. As such, we do not request the GPU to answer this
   * request: we have the answer on the CPU, which would be faster.
   * @name The attribute name as it appear in the vertex shader source
   * @return The attribute location, -1 if there is no attribute with such name in this program
   */
  int getAttributeLocation( const std::string& name ) const;


  unsigned int programId();

private:

  void resources_introspection();

  unsigned int m_programId;
  std::unordered_map< std::string, int > m_uniforms;
  std::unordered_map< std::string, int > m_attributes;
  std::string m_vertexFilename;
  std::string m_fragmentFilename;
};

typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr; /*!< Typedef for a smart pointer of ShaderProgram */

#endif
