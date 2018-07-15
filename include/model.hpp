#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <geometry.hpp>
#include <vector>
#include <materials/color_material.hpp>
#include <shapes/triangle.hpp>
#include <material_factory.hpp>


class Model {
public:
    Model(const char* filename, Material* mat, glm::vec3 loc, glm::vec3 rot, glm::vec3 s=glm::vec3(1.0f, 1.0f, 1.0f)) {
        MaterialFactory factory;

        glm::mat4 trans(1);
        trans = glm::translate(trans, loc);
        trans = glm::rotate(trans, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
        trans = glm::rotate(trans, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
        trans = glm::rotate(trans, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::scale(trans, s);


        // std::cout << "File: " << filename << std::endl;
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate);
        if(!scene) {
            std::cout << "ERROR::READING SCENE: " <<filename << std::endl;
        }
        // std::cout << "Model: " << scene->mNumMeshes << std::endl;

        for(int i = 0;i < scene->mNumMeshes;i++) {
            // std::cout << "Mesh: " << i << "/" << scene->mNumMeshes << std::endl;
            aiMesh* mesh = scene->mMeshes[i];
            aiVector3D* verts = mesh->mVertices;
            for(int j = 0;j < mesh->mNumFaces;j++) {
                Material* new_mat = factory.create(mat);
                // std::cout << "Face: " << j << "/" << mesh->mNumFaces << std::endl;
                aiFace face = mesh->mFaces[j];

                aiVector3D a = verts[face.mIndices[0]];
                aiVector3D b = verts[face.mIndices[1]];
                aiVector3D c = verts[face.mIndices[2]];

                glm::vec3 A = glm::vec3(a.x, a.y, a.z);
                glm::vec3 B = glm::vec3(b.x, b.y, b.z);
                glm::vec3 C = glm::vec3(c.x, c.y, c.z);
                glm::vec4 A1(A.x, A.y, A.z, 1.0f);
                glm::vec4 B1(B.x, B.y, B.z, 1.0f);
                glm::vec4 C1(C.x, C.y, C.z, 1.0f);

                A1 = trans * A1;
                B1 = trans * B1;
                C1 = trans * C1;
                A = glm::vec3(A1.x, A1.y, A1.z);
                B = glm::vec3(B1.x, B1.y, B1.z);
                C = glm::vec3(C1.x, C1.y, C1.z);

                if(mesh->HasNormals() && !mesh->HasTextureCoords(0)) {
                    aiVector3D* normals = mesh->mNormals;
                    aiVector3D an = normals[face.mIndices[0]];
                    aiVector3D bn = normals[face.mIndices[1]];
                    aiVector3D cn = normals[face.mIndices[2]];

                    glm::vec3 An = glm::vec3(an.x, an.y, an.z);
                    glm::vec3 Bn = glm::vec3(bn.x, bn.y, bn.z);
                    glm::vec3 Cn = glm::vec3(cn.x, cn.y, cn.z);
                    shapes.push_back(new Triangle(new_mat, A, B, C, An, Bn, Cn));
                }
                else if(!mesh->HasNormals() && mesh->HasTextureCoords(0)) {
                    aiVector3D* textureCoords = mesh->mTextureCoords[0];
                    aiVector3D at = textureCoords[face.mIndices[0]];
                    aiVector3D bt = textureCoords[face.mIndices[1]];
                    aiVector3D ct = textureCoords[face.mIndices[2]];

                    glm::vec2 At = glm::vec2(at.x, at.y);
                    glm::vec2 Bt = glm::vec2(bt.x, bt.y);
                    glm::vec2 Ct = glm::vec2(ct.x, ct.y);
                    shapes.push_back(new Triangle(new_mat, A, B, C, At, Bt, Ct));
                }
                else if(mesh->HasNormals() && mesh->HasTextureCoords(0)) {
                    aiVector3D* normals = mesh->mNormals;
                    aiVector3D an = normals[face.mIndices[0]];
                    aiVector3D bn = normals[face.mIndices[1]];
                    aiVector3D cn = normals[face.mIndices[2]];

                    glm::vec3 An = glm::vec3(an.x, an.y, an.z);
                    glm::vec3 Bn = glm::vec3(bn.x, bn.y, bn.z);
                    glm::vec3 Cn = glm::vec3(cn.x, cn.y, cn.z);

                    aiVector3D* textureCoords = mesh->mTextureCoords[0];
                    aiVector3D at = textureCoords[face.mIndices[0]];
                    aiVector3D bt = textureCoords[face.mIndices[1]];
                    aiVector3D ct = textureCoords[face.mIndices[2]];

                    glm::vec2 At = glm::vec2(at.x, at.y);
                    glm::vec2 Bt = glm::vec2(bt.x, bt.y);
                    glm::vec2 Ct = glm::vec2(ct.x, ct.y);

                    shapes.push_back(new Triangle(new_mat, A, B, C, An, Bn, Cn, At, Bt, Ct));

                }
                else {
                    shapes.push_back(new Triangle(new_mat, A, B, C));
                }
            }
        }
        delete mat;
    }

    std::vector<Geometry*> getShapes() {
        return shapes;

    }


private:
    std::vector<Geometry*> shapes;
    // glm::vec3 translate;
    // glm::vec3 rotate;
    // glm::vec3 scale;
};

#endif
