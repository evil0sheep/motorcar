#ifndef SCENEGRAPHNODE_H
#define SCENEGRAPHNODE_H

#include <QObject>
#include <QtDebug>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SceneGraphNode : public QObject
{
    Q_OBJECT
public:
    explicit SceneGraphNode(QObject *parent = 0); 
    virtual ~SceneGraphNode();
    virtual bool animate(float time) = 0;
    virtual bool draw() = 0;
    virtual void traverseChildren(float deltaTime);

    
    bool traverse(float time);
    SceneGraphNode *getParentNode() const;
    void setParentNode(SceneGraphNode *parent);
    void addChildNode(SceneGraphNode *child);
    glm::mat4 getWorldTransform();



private:
    glm::mat4 mTransform;
    SceneGraphNode *mParentNode;
    QList<SceneGraphNode *> mChildren;


signals:
    
public slots:
    
};

#endif // SCENEGRAPHNODE_H
