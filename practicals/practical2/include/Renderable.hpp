#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <glm/glm.hpp>
#include <unordered_set>
#include <memory>
#include <vector>

#include "ShaderProgram.hpp"
#include <SFML/Graphics.hpp>

/**
 * @brief Renderable interface.
 *
 * This is the base class for all 3D renderable objects of the project.
 *
 * \note
 * Some words about virtuality (http://www.gotw.ca/publications/mill18.htm).
 * Prefer to make base class virtual functions private (or protected if you
 * really must). This separates the concerns of interface and implementation,
 * which stabilizes interfaces and makes implementation decisions easier to
 * change and refactor later. Here are some guidelines:
 * \li #1 Prefer to make interfaces nonvirtual, using Template Method.
 * \li #2 Prefer to make virtual functions private.
 * \li #3 Only if derived classes need to invoke the base implementation of a
 * virtual function, make the virtual function protected.
 * \li #4 A base class destructor should be either public and virtual, or
 * protected and nonvirtual. In the first case, you can do something like
 * <tt> delete basePtr</tt>, while in the second case, you can not.
 */
class Renderable
{
public:
    /** @name Construction/Destruction of renderable instances.*/
    /** \brief Renderable destructor.
     *
     * Some smart pointers to this base class could be stored, such as in
     * Viewer. As such, the destructor needs to be public and virtual (guideline
     * #4).
     */
    virtual ~Renderable();

    /** \brief Renderable default constructor.
     *
     * Initialize the model matrix \ref m_model to identity.
     * Set the shader program \ref m_shaderProgram to \c nullptr.
     */
    Renderable();

    /** \brief Renderable program constructor.
     *
     * Initialize the model matrix \ref m_model to identity.
     * Set the shader program \ref m_shaderProgram to \a program.
     * \param program Shader program to use to draw this.
     */
    Renderable(ShaderProgramPtr program);

    /** @name Public interface */
    /** @brief Set the model matrix.
     *
     * Define the model matrix.
     * @param model New model matrix
     */
    void setModelMatrix( const glm::mat4& model );

    /** @brief Get the model matrix.
     *
     * Return the model matrix.
     */
    const glm::mat4& getModelMatrix() const;

    /** @brief Change the shader program.
     *
     * Set a new shader program to use for the rendering.
     * @param prog New shader program
     */
    void setShaderProgram( ShaderProgramPtr prog );


    /** @name Public interface used by Viewer.
     * Those functions are automatically called by the Viewer on added Renderable.
     * You are not expected to call them directly. */
    /** \brief Bind the shader program on the GPU.
     */
    void bindShaderProgram();

    /** \brief Unbind the shader program of the GPU.
     */
    void unbindShaderProgram();
    /**
     * \brief Get the location of the projection matrix in the shader program.
     * \return the location of the projection matrix uniform or -1 if the
     * associated shader program does not use such an uniform ("projMat").
     */
    int projectionLocation();

    /** \brief Get the location of the view matrix in the shader program.
     * \return the location of the view matrix uniform or -1 if the associated
     * shader program does not use such an uniform ("viewMat").
     */
    int viewLocation();
    /** \brief Draw this renderable.
     *
     * This function calls the private pure virtual function <tt> do_draw() </tt>
     * (guidelines #1 and #2). When this function is called by the Viewer,
     * the shader program is already binded, the view and projection matrices are
     * already set.
     */
    void draw();

    /** \brief Animate this renderable.
     *
     * This function calls the private pure virtual function <tt> do_animate(time) </tt>
     * (guidelines #1 and #2). The simulation time equals zero when the animation is
     * started (see Viewer::startAnimation()).
     * \param time Current simulation time.
     */
    void animate( float time );

    /**
     * \brief keyPressedEvent
     * \param e
     */
    void keyPressedEvent(sf::Event& e);

    /**
     * \brief keyReleasedEvent
     * \param e
     */
    void keyReleasedEvent(sf::Event& e);

    /**
     * \brief mousePressEvent
     * \param e
     */
    void mousePressEvent(sf::Event& e);

    /**
     * \brief mouseReleaseEvent
     * \param e
     */
    void mouseReleaseEvent(sf::Event& e);

    /**
     * \brief mouseWheelEvent
     * \param e
     */
    void mouseWheelEvent(sf::Event& e);

    /**
     * \brief mouseMoveEvent
     * \param e
     */
    void mouseMoveEvent(sf::Event& e);

private:
    /** @name Private viewer interface.
     * According to guideline #1, we have used the Template method. Now we are
     * using guideline #2 for virtual methods.*/
    /** \brief Draw virtual function.
     */
    virtual void do_draw() = 0;
    /** \brief Animate virtual function.
     * \param time The current simulation time.
     */
    virtual void do_animate( float time ) = 0;

    virtual void do_keyPressedEvent(sf::Event& e);
    virtual void do_keyReleasedEvent(sf::Event& e);
    virtual void do_mousePressEvent(sf::Event& e);
    virtual void do_mouseReleaseEvent(sf::Event& e);
    virtual void do_mouseWheelEvent(sf::Event& e);
    virtual void do_mouseMoveEvent(sf::Event& e);

protected:
    /** @name Protected members.
     *
     * We want those members to be accessible in the derived classes.
     */
    glm::mat4 m_model; /*!< Model matrix of the renderable. */
    ShaderProgramPtr m_shaderProgram; /*!< Shader program of the renderable. */
};
typedef std::shared_ptr<Renderable> RenderablePtr; /*!< Typedef for smart pointer to renderable.*/

#endif
