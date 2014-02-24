#include "sixensebasenode.h"

using namespace motorcar;
SixenseBaseNode::SixenseBaseNode(int baseIndex, PhysicalNode *parent, const glm::mat4 &transform)
    : PhysicalNode(parent, transform)
    , m_baseIndex(baseIndex)
    , m_connected(true)
{

    sixenseAllControllerData acd;
    FILE *log_file = stdout;



    sixenseSetActiveBase(baseIndex);

    sixenseGetAllNewestData( &acd );


    for(int cont=0; cont<sixenseGetMaxControllers(); cont++ ) {

        if( sixenseIsControllerEnabled( cont ) ) {

            fprintf( log_file, "base: %d controller: %d ", baseIndex, cont );

            fprintf( log_file, "pos: %f %f %f ", acd.controllers[cont].pos[0], acd.controllers[cont].pos[1], acd.controllers[cont].pos[2] );
            fprintf( log_file, "rot_mat: %f %f %f  %f %f %f  %f %f %f",
                     acd.controllers[cont].rot_mat[0][0], acd.controllers[cont].rot_mat[0][1], acd.controllers[cont].rot_mat[0][2],
                    acd.controllers[cont].rot_mat[1][0], acd.controllers[cont].rot_mat[1][1], acd.controllers[cont].rot_mat[1][2],
                    acd.controllers[cont].rot_mat[2][0], acd.controllers[cont].rot_mat[2][1], acd.controllers[cont].rot_mat[2][2] );

            fprintf( log_file, "\n" );
            fflush(log_file);


            m_controllers.push_back(new SixenseControllerNode(cont, scene()));

        }

    }
}

void SixenseBaseNode::traverseNode(Scene *scene, long deltaMillis)
{
    PhysicalNode::traverseNode(scene, deltaMillis);

    sixenseAllControllerData acd;

    if(sixenseIsBaseConnected(m_baseIndex)){
        if(!connected()){
            setConnected(true);
            std::cout << "Sixsense base " << m_baseIndex << " reconnected" << std::endl;
        }
        sixenseSetActiveBase(m_baseIndex);
        sixenseGetAllNewestData( &acd );

        for(SixenseControllerNode *controller : m_controllers){
            if(sixenseIsControllerEnabled(controller->controllerIndex())){
                if(!controller->enabled()){
                    controller->setEnabled(true);
                    std::cout << "Sixsense controller "<< controller->controllerIndex() << " re-enabled" << std::endl;
                }

                sixenseControllerData data = acd.controllers[controller->controllerIndex()];

                controller->updateState(data);

                glm::mat3 rotation = glm::make_mat3((float *)data.rot_mat);
                glm::vec3 position = (glm::make_vec3(data.pos) / 1000.f);

                glm::mat4 controllerTransform = glm::translate(glm::mat4(), position) *
                                    glm::mat4(glm::vec4(rotation[0], 0),
                                              glm::vec4(rotation[1], 0),
                                              glm::vec4(rotation[2], 0),
                                              glm::vec4(0,0,0, 1));

//                if(controller->controllerIndex() != 0){
//                    controller->setWorldTransform(glm::translate(glm::mat4(), glm::vec3(0,0.1,0)));
//                else
                    controller->setWorldTransform(this->inverseWorldTransform() * controllerTransform);




            }else if (controller->enabled()){
                controller->setEnabled(false);
                std::cout << "Sixsense controller "<< controller->controllerIndex() << " disabled" << std::endl;
            }
        }

    }else if(connected()){
       setConnected(false);
       std::cout << "Sixsense base " << m_baseIndex << " disconnected" << std::endl;
    }

}

bool SixenseBaseNode::connected() const
{
    return m_connected;
}

void SixenseBaseNode::setConnected(bool connected)
{
    m_connected = connected;
}

std::vector<SixenseControllerNode *> SixenseBaseNode::controllers() const
{
    return m_controllers;
}


