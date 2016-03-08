#include <scene/mesh.h>
#include <tinyobj/tiny_obj_loader.h>
#include <iostream>
#include <algorithm>
#include <set>

Mesh::Mesh() : Geometry(),
    bufInfluences(QOpenGLBuffer::VertexBuffer),
    bufJointIDs(QOpenGLBuffer::VertexBuffer) //Makes no difference if I initialize or not
{}

void Mesh::destroy() {
    Geometry::destroy();
    bufInfluences.destroy();
    bufJointIDs.destroy();
    faces.clear();
    vertices.clear();
}

void Mesh::create()
{
    std::vector<GLuint> idx;
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> nor;

    std::vector<glm::vec4> col;

    GLuint index = 0;
    for(Face* f : faces)
    {
        glm::vec3 p0, p1, p2;
        p0 = f->vertices[0]->position;
        p1 = f->vertices[1]->position;
        p2 = f->vertices[2]->position;

        glm::vec3 e1 = p0 - p1;
        glm::vec3 e2 = p1 - p2;
        glm::vec4 n = glm::vec4(glm::cross(e1, e2), 0.0f);

        for(int i = 0; i < 3; i++)
        {
            idx.push_back(index++);
            pos.push_back(glm::vec4(f->vertices[i]->position, 1.0f));
            nor.push_back(n);

            col.push_back(glm::vec4(0.0f,1.0f,0.9f,1.0f));
//          col.push_back(glm::vec4(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),static_cast <float> (rand()) / static_cast <float> (RAND_MAX),static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 1.0f));
        }
    }

    count = idx.size();

//    this->color = glm::vec4(0.0f, 1.0f, 0.9f, 1.0f);

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(col.data(), col.size() * sizeof(glm::vec4));

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(idx.data(), idx.size() * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(pos.data(), pos.size() * sizeof(glm::vec4));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(nor.data(), nor.size() * sizeof(glm::vec4));
}

bool Mesh::LoadOBJ(const QString &filepath, GLWidget277* mygl)
{
    QHash<QString, Vertex*> pos_to_vert_map;
    QHash<unsigned int, Vertex*> index_to_vert_map;
    unsigned int max_vertex_index = 0;
    //Each shape_t is a mesh within the OBJ file. The OBJs we provide only contain one shape each
    //The OBJ's materials are not handled by this loader
    std::vector<tinyobj::shape_t> shapes; std::vector<tinyobj::material_t> materials;
    std::string errors = tinyobj::LoadObj(shapes, materials, filepath.toStdString().c_str());
    std::cout << errors << std::endl;
    //map of QString -> V*
//    for each vert loaded, QString::asprintf(%f%f%f, pos.x, pos.y, pos.z)
    //before adding to vertex array, check if vertex exists
    //if already exists, put its index as the key into a map of int to Vert*, where the original Vert* is the value
    //Else do the same but create a new Vert*
    //In short, use a list of V* where multiple indices may contain a pointer to the same Vertex

    if(errors.size() == 0)
    {
        //Read the information from the vector of shape_ts
        for(unsigned int i = 0; i < shapes.size(); i++)
        {
            std::vector<float> &positions = shapes[i].mesh.positions;
            //Fill our Mesh's list of vertices
            for(unsigned int j = 0; j < positions.size(); j+=3)
            {
                glm::vec3 pos(positions[j], positions[j+1], positions[j+2]);
                QString pos_string = QString("%1 %2 %3").arg(pos.x).arg(pos.y).arg(pos.z);
                Vertex* vert = nullptr;
                if(pos_to_vert_map.contains(pos_string))
                {
                    vert = pos_to_vert_map.value(pos_string);
                }
                else
                {
                    vert = new Vertex(pos);
                    this->vertices.append(vert);
                    pos_to_vert_map.insert(pos_string, vert);
                }
                index_to_vert_map.insert(max_vertex_index++, vert);
            }

            //You may add normals and UV coordinates to your meshes if you like, but they are not required
            std::vector<float> &normals = shapes[i].mesh.normals;
            std::vector<float> &uvs = shapes[i].mesh.texcoords;

            //Tell our faces which vertices they use
            std::vector<unsigned int> &indices = shapes[i].mesh.indices;
            for(unsigned int j = 0; j < indices.size(); j += 3)
            {
                unsigned int i1 = indices[j]; Vertex* v1 = index_to_vert_map.value(i1);
                unsigned int i2 = indices[j+1]; Vertex* v2 = index_to_vert_map.value(i2);
                unsigned int i3 = indices[j+2]; Vertex* v3 = index_to_vert_map.value(i3);
                this->faces.append(new Face(v1, v2, v3));
            }


            std::map<std::pair<int, int>, HalfEdge* > edges;

            for (Face* f: faces) {
                //Creating the initial half edge meshes (.face and .vert initialized)
                for (int i = 0; i < 3; i++) {
                    HalfEdge* he = new HalfEdge(f,f->vertices[i]);
                    emit mygl->sig_sendHalfEdge(he); //sends it to mygl to send to QListWidget
                    if (i == 2) {
                        f->start_edge = he;
                        f->vertices[i]->edge = he;
                        edges.insert(std::pair<std::pair<int, int>, HalfEdge*>(std::pair<int,int>(f->vertices[i]->id,f->vertices[0]->id), he));
                    } else {
                        edges.insert(std::pair<std::pair<int, int>, HalfEdge*>(std::pair<int,int>(f->vertices[i]->id,f->vertices[i+1]->id),
                                he));
                        f->vertices[i]->edge = he;
                    }
                }

                //Setting the ".next" information (also sets the .sym)
                for (int i = 0; i < 3; i++) {
                    if (i== 0) {
                        edges.at(std::pair<int,int>(f->vertices[i]->id,f->vertices[i+1]->id))->next =
                                edges.at(std::pair<int, int>(f->vertices[i+1]->id,f->vertices[i+2]->id));
                    } else if (i == 1){
                        edges.at(std::pair<int,int>(f->vertices[i]->id,f->vertices[i+1]->id))->next =
                                edges.at(std::pair<int, int>(f->vertices[i+1]->id,f->vertices[0]->id));
                    } else if (i == 2) {
                        edges.at(std::pair<int,int>(f->vertices[i]->id,f->vertices[0]->id))->next =
                                edges.at(std::pair<int, int>(f->vertices[0]->id,f->vertices[1]->id));
                    }

                    //Setting the symmetrical edges to each other (pair of i, i+1).sym = pair (i+1, i)
                    if (i == 2) {
                        if (edges.find(std::pair<int,int>(f->vertices[0]->id,f->vertices[i]->id)) != edges.end()) {
                            edges.at(std::pair<int,int>(f->vertices[i]->id, f->vertices[0]->id))->sym =
                                    edges.at(std::pair<int,int>(f->vertices[0]->id,f->vertices[i]->id));

                            edges.at(std::pair<int,int>(f->vertices[0]->id,f->vertices[i]->id))->sym =
                                    edges.at(std::pair<int,int>(f->vertices[i]->id,f->vertices[0]->id));
                        }
                    } else if (edges.find(std::pair<int,int>(f->vertices[i+1]->id,f->vertices[i]->id)) != edges.end()) {
                        edges.at(std::pair<int,int>(f->vertices[i]->id,f->vertices[i+1]->id))->sym =
                                edges.at(std::pair<int,int>(f->vertices[i+1]->id,f->vertices[i]->id));

                        edges.at(std::pair<int,int>(f->vertices[i+1]->id,f->vertices[i]->id))->sym =
                                edges.at(std::pair<int,int>(f->vertices[i]->id,f->vertices[i+1]->id));
                    }

                } //End ".next" for loop
            }//End for (Face f : faces)


            //504
            sendVertices(mygl);
            sendFaces(mygl);
//            sendHalfEdges(mygl,edges); //Old code that sends half edges in random edges
        }
        create();
        return true;
        //If you wish, you may try to implement .mtl file loading
    }
    else
    {
        //An error loading the OBJ occurred!
        std::cout << errors << std::endl;
        return false;
    }
}

void getDistances(Vertex* v, Joint* j, Joint* (&joints)[2], float (&distances)[2]) {
   j->setBindMatrix();

  float distance = glm::distance(glm::vec4(v->position,1),j->getOverallTransformation() * glm::vec4(0,0,0,1));
  float max = std::max(distances[0],distances[1]);
  if (distance < max) {
      if (distances[0] == max) {
          distances[0] = distance;
          joints[0] = j;
      } else {
          distances[1] = distance;
          joints[1] = j;
      }
  }

  //Recurse through children
  if (j->getChildren().size() > 0) {
      for (int i=0; i<j->getChildren().size();i++) {
          getDistances(v,j->getChildren()[i],joints,distances);
      }
  }
}

void Mesh::bindVertices(Joint* root){
    std::vector<glm::vec2> influences;
    std::vector<glm::ivec2> ids;

    for (int i=0; i<vertices.size();i++) {
        Joint* joints[2];
        if (root->getID() ==13 && vertices[i]->id > 206 && vertices[i]->id<387) {
            int x = 1;
        }
        float distances[2]{FLT_MAX,FLT_MAX};

        getDistances(vertices[i],root,joints,distances);

        float total    = distances[0]+distances[1];
        distances[0]  /= total;
        distances[1]  /= total;

        vertices[i]->bindPosition = vertices[i]->position;

        vertices[i]->influences[0] = std::pair <Joint*,float> (joints[0],distances[0]);
        vertices[i]->influences[1] = std::pair <Joint*,float> (joints[1],distances[1]);

    }


    for(Face* f : this->faces)
    {
        for(int i = 0; i < 3; i++)
        {
            influences.push_back(glm::vec2(f->vertices[i]->influences[0].second,f->vertices[i]->influences[1].second));
            ids.push_back(glm::ivec2(f->vertices[i]->influences[0].first->getID(),
                    f->vertices[i]->influences[1].first->getID()));

        }
        //Visit each vertex and send its data to the vector
    }

    //Create the buffer objects for influences and joints:
    bufInfluences.create();
    bufInfluences.bind();
    bufInfluences.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufInfluences.allocate(influences.data(), influences.size() * sizeof(glm::vec2));

    bufJointIDs.create();
    bufJointIDs.bind();
    bufJointIDs.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufJointIDs.allocate(ids.data(), ids.size() * sizeof(glm::ivec2));

}

bool Mesh::bindInfluences() {
    return bufInfluences.bind();
}

bool Mesh::bindJointIDs() {
    return bufJointIDs.bind();
}

void Mesh::updateVertices(Joint* root, MeshShaderProgram &prog_mesh) {
    //Update The TransMats
    glm::mat4 transMats[100];
    root->getTransMats(transMats);

    prog_mesh.setTransMatrices(transMats);
}

void Mesh::sendVertices(GLWidget277* mygl) {
    for (int i=0; i < vertices.size(); i++) {
         emit mygl->sig_sendVertex(vertices[i]);
    }
}

void Mesh::sendFaces(GLWidget277* mygl) {
    for (int i=0; i < faces.size(); i++) {
         emit mygl->sig_sendFace(faces[i]);
    }
}
