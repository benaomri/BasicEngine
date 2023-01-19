#pragma once
#include "shader.h"
#include "shape.h"
#include "camera.h"
#include "VertexArray.hpp"
#include <vector>


class Scene : public MovableGLM
{

public:
    enum axis{xAxis,yAxis,zAxis};
    enum transformations{xTranslate, yTranslate, zTranslate, xRotate, yRotate, zRotate, xScale, yScale, zScale, xCameraTranslate, yCameraTranslate, zCameraTranslate};
    enum modes{POINTS, LINES, LINE_LOOP, LINE_STRIP, TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN, QUADS};
    enum buffers{COLOR, DEPTH, STENCIL, BACK, FRONT, ACCUM, NONE };
    enum shapes{Axis, Plane, Cube, Octahedron, Tethrahedron, LineCopy, MeshCopy};

    Scene();
    Scene(float angle,float relationWH,float near, float far);

    void AddShapeFromFile(const std::string& fileName,int parent,unsigned int mode);
    virtual void AddShape(int type,int parent,unsigned int mode);
    void AddShapeCopy(int indx,int parent,unsigned int mode);

    void AddShader(const std::string& fileName);
    void AddTexture(const std::string& textureFileName, bool for2D);
    void AddTexture(int width,int height, unsigned char *data);

    void AddCamera(glm::vec3& pos , float fov, float relationWH, float zNear, float zFar);

    void ZeroShapesTrans();

    virtual void Update(const glm::mat4 &MVP,const glm::mat4 &Normal,const int  shaderIndx) = 0;
    virtual void WhenTranslate(){};
    virtual void WhenRotate(){};
    virtual void Motion(){};
    virtual void Draw(int shaderIndx,int cameraIndx,int buffer,bool toClear,bool debugMode);
    virtual ~Scene(void);

    void MoveCamera(int cameraIndx,int type,float amt);
    void ShapeTransformation(int type,float amt);

    float Picking(int x,int y);
    void Resize(int width,int hight);

    inline void SetParent(int indx,int newValue) {chainParents[indx]=newValue;}
    inline float GetNear(int cameraIndx){return cameras[cameraIndx]->GetNear();}
    inline float GetFar(int cameraIndx){return cameras[cameraIndx]->GetFar();}

    void ReadPixel();

    inline float GetAngle(int cameraIndx) {return cameras[cameraIndx]->GetAngle();}
    inline void Activate() {isActive = true; }
    inline void Deactivate() {isActive = false;}
    void HideShape(int shpIndx);
    void UnhideShape(int shpIndx);

    void UpdatePosition(float xpos, float ypos);
    virtual void MouseProccessing(int button);
    bool inline IsActive() const { return isActive;}

    inline void SetShapeTex(int shpIndx,int texIndx){shapes[shpIndx]->SetTexture(texIndx);}
    inline void SetShapeShader(int shpIndx,int shdrIndx){shapes[shpIndx]->SetShader(shdrIndx);}

    // Added Functions
    void mainParameters(int DISPLAY_WIDTH, int DISPLAY_HEIGHT, float CAMERA_ANGLE, float NEAR, float FAR) {
        width = DISPLAY_WIDTH;
        height = DISPLAY_WIDTH;
        cameraAngle = CAMERA_ANGLE;
        cameraNear = NEAR;
        cameraFar = FAR;
    }
    virtual std::vector<Shape*>* getShapes() = 0;
    virtual void addBezier1DShape(Shape* bezier_1D_line, int parent) = 0;
    virtual void mouseScrolling(glm::vec3 delta, int mode) = 0;

    glm::vec2 zBufferConverter(float x, float y, int mode);

    inline int getXrel() { return xrel; }
    inline int getYrel() { return yrel; }
    inline float getXPicked() { return xPicked; }
    inline float getYPicked() { return yPicked; }
    inline int getWidth() { return width; }
    inline int getHeight() { return height; }
    void setXPicked(float x_picked) { this->xPicked = x_picked; }
    void setYPicked(float y_picked) { this->yPicked = y_picked; }
    void setPickedShape(int pickedShape) { this->pickedShape = pickedShape; }

private:

    std::vector<Camera*> cameras;

    float depth;
    int xold, yold,xrel, yrel;
    int cameraIndx;
    void Clear(float r, float g, float b, float a);

    int width, height;
    float cameraAngle, cameraNear, cameraFar;
    float pickedDepth, xPicked, yPicked;

protected:
    std::vector<Shape*> shapes;
    std::vector<Shader*> shaders;
    std::vector<int> chainParents;
    std::vector<Texture*> textures;

    int pickedShape;

    bool isActive;
};

