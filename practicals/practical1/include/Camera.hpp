#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
/**
 * @brief Manage the Camera.
 *
 * We consider a camera to be defined by two 4x4 matrices:
 *  - the view matrix
 *  - the projection matrix.
 *
 * The view matrix defines the position and the orientation of the camera in the
 * world coordinates. It is represented as follow:
 * | Column 0 | Column 1 | Column 2 | Column 3 |
 * |----------|----------|----------|----------|
 * |    X0    |    X1    |    X2    |    P0    |
 * |    Y0    |    Y1    |    Y2    |    P1    |
 * |    Z0    |    Z1    |    Z2    |    P2    |
 * |    0     |    0     |    0     |    1     |
 * X=(X0,X1,X2), Y=(Y0,Y1,Y2) and Z=(Z0,Z1,Z2) represent the right, up, and
 * backward direction of the camera in the world coordinates. The point
 * P=(P0,P1,P2) is the position of the world origin in the camera frame
 * (X,Y,Z). Thus, the point (-dot(P,X), -dot(P,Y), -dot(P,Z)) is the position
 * of the camera in the world coordinates.
 *
 * The projection matrix defines the way the scene viewed by the camera is
 * transformed (projected) to a 2D image that will be displayed on screen. To
 * compute this matrix, we need ratio of the 2D image, the field of view and
 * the far and near cutting plane distances.
 */
class Camera
{
public:
    Camera();
    ~Camera();

    /** @brief Animate the camera
     *
     * This function is currently empty, but you can define an animation
     * behavior here. This function is automatically called by the Viewer
     * @param time Current simulation time.
     */
    void animate( float time );
    /**
     * @name Camera View Matrix
     * Set of methods which interact with the camera view matrix. This matrix
     * which determines the camera frame in the world coordinates, i.e. the way
     * the camera is positioned and oriented.
     * @{
     */
    /**
     * Returns the current view matrix: where the camera looks.
     */
    const glm::mat4& viewMatrix() const;

    /**
     * Returns the camera position in world coordinates.
     */
    glm::vec3 getPosition() const;

    /**
     * Returns the camera right direction (+X) in world coordinates.
     */
    glm::vec3 getRight() const;

    /**
     * Returns the camera up direction (+Y) in world coordinates.
     */
    glm::vec3 getUp() const;

    /**
     * Returns the camera forward direction (-Z / looking direction) in world coordinates.
     */
    glm::vec3 getForward() const;

    /**
     * Sets the camera position in world coordinates.
     * @param pos New camera world position.
     */
    void setPosition( const glm::vec3& pos );

    /**
     * Sets the camera right direction in world coordinates.
     * @param right New camera right axis.
     */
    void setRight( const glm::vec3& right );

    /** Set the camera up direction in world coordinates.
     * @param up New camera up axis.
     */
    void setUp( const glm::vec3& up );
    /** Set the camera forward (-Z) direction in world coordinates.
     * @param forward New camera forward direction.
     */
    void setForward( const glm::vec3& forward );
    ///@}


    /**
    * @name Camera Projection Matrix
    * Set of methods which interact with the projection matrix. This matrix
    * determine how a scene is projected on a 2D image that will be displayed
    * on screen.
    * @{
    */
    /**
    * Returns the projection matrix.
    */
    const glm::mat4& projectionMatrix() const;

    /**
    * Returns the field of view.
    */
    float fov() const;

    /**
    * Returns the length ratio of the image taken by this camera:
    */
    float ratio() const;

    /**
    * Returns the near clipping plane: anything closer to the camera than this plane
    * will be removed from the view.
    */
    float znear() const;

    /**
    * Returns the far clipping plane: anything farther from the camera than this plane
    * will be removed from the view.
    */
    float zfar() const;

    /**
    * Sets the field of view. 1.04 is a good start.
    */
    void setFov( const float& v );

    /**
    * Sets the ratio of the camera (generally, this is done at each window resize).
    */
    void setRatio( const float& v );

    /**
    * Sets the far clipping plane.
    */
    void setZfar( const float& v );

    /**
    * Sets the near clipping plane.
    */
    void setZnear( const float& v );
    ///@}


    /**
     * How the camera react to a mouse displacement.
     */
    enum CAMERA_MOUSE_BEHAVIOR {
      /** The view matrix is modified to turn the camera around the world origin.*/
      ARCBALL_BEHAVIOR,
      /** The view matrix is modified to point its 'nose' in the direction pointed by the mouse. */
      SPACESHIP_BEHAVIOR
    };

    /** @name Mouse Control
     * Control the camera with the mouse.
     * @{
     */
    /**
    * Returns the mouse behavior.
    */
    CAMERA_MOUSE_BEHAVIOR getMouseBehavior() const;

    /**
    * Sets the mouse behavior.
    */
    void setMouseBehavior( const CAMERA_MOUSE_BEHAVIOR& v );
    /**
     * Update the camera view matrix according to the current mouse behavior.
     */
    void update( float dx, float dy );
    ///@}
private:

    /** @name Private members */
    CAMERA_MOUSE_BEHAVIOR m_mouseBehavior;
    float m_fov;
    float m_ratio;
    float m_znear;
    float m_zfar;
    glm::mat4 m_view;
    glm::mat4 m_projection;
};

#endif
