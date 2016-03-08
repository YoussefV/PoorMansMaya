#include "meshshaderprogram.h"

void MeshShaderProgram::create(const char *vertfile, const char *fragfile) {
    ShaderProgram::create(vertfile,fragfile);

    attrInfluences = prog.attributeLocation("weights");
    attrJointIDs = prog.attributeLocation("joint_IDs");

    for (int i=0; i<100; i++) {
        unifTransMats[i] = prog.uniformLocation(QString("u_TransMats[" + QString::number(i) + "]"));
        unifBindMats[i] = prog.uniformLocation(QString("u_BindMats[" + QString::number(i) + "]"));
    }

    unifSelectedID = prog.uniformLocation("u_selectedJoint");
    prog.bind();
    prog.setUniformValue(unifSelectedID,-1);

}

void MeshShaderProgram::setSelectedID(int id) {
    prog.bind();
    prog.setUniformValue(unifSelectedID, id);
}

void MeshShaderProgram::setBindMatrices(glm::mat4 bindMats[100]){
    prog.bind();

    for (int i =0; i<100; i++) {
        if (unifBindMats[i] != -1) {
            QMatrix4x4 test = la::to_qmat(bindMats[i]);
            prog.setUniformValue(unifBindMats[i],la::to_qmat(bindMats[i]));
        }
    }

//    QMatrix4x4 newArr[100];
//    for (int i =0; i<100; i++) {
//        if (unifBindMats != -1) {
//            QMatrix4x4 test = la::to_qmat(bindMats[i]);
//            newArr[i] = test;
//        }
//    }
//    prog.setUniformValueArray("unifBindMats",newArr,100);
}

void MeshShaderProgram::setTransMatrices(glm::mat4 transMats[100]) {
    prog.bind();

    for (int i =0; i<100; i++) {
        if (unifTransMats[i] != -1) {
            QMatrix4x4 test = la::to_qmat(transMats[i]);
            prog.setUniformValue(unifTransMats[i],la::to_qmat(transMats[i]));
        }
    }

//    QMatrix4x4 newArr[100];
//    for (int i =0; i<100; i++) {
//        if (unifTransMats != -1) {
//            QMatrix4x4 test = la::to_qmat(transMats[i]);
//            newArr[i] = test;
//        }
//    }
//    prog.setUniformValueArray("unifTransMats",newArr,100);
}


void MeshShaderProgram::draw(GLWidget277 &f, Mesh &d) {
    prog.bind();

    //Send the Drawable's color to the shader
    if(unifColor != -1){
        prog.setUniformValue(unifColor, la::to_qvec(d.getColor()));
    }

    // Each of the following blocks checks that:
    //   * This shader has this attribute, and
    //   * This Drawable has a vertex buffer for this attribute.
    // If so, it binds the appropriate buffers to each attribute.

    if (attrPos != -1 && d.bindPos()) {
        prog.enableAttributeArray(attrPos);
        f.glVertexAttribPointer(attrPos, 4, GL_FLOAT, false, 0, NULL);
    }

    if (attrNor != -1 && d.bindNor()) {
        prog.enableAttributeArray(attrNor);
        f.glVertexAttribPointer(attrNor, 4, GL_FLOAT, false, 0, NULL);
    }

    if (attrCol != -1 && d.bindCol()) {
        prog.enableAttributeArray(attrCol);
        f.glVertexAttribPointer(attrCol, 4, GL_FLOAT, false, 0, NULL);
    }

    if (attrInfluences != -1 && d.bindInfluences()) {
        prog.enableAttributeArray(attrInfluences);
        f.glVertexAttribPointer(attrInfluences, 2, GL_FLOAT, false, 0, NULL);
    }

    if (attrJointIDs != -1 && d.bindJointIDs()) {
        prog.enableAttributeArray(attrJointIDs);
        f.glVertexAttribIPointer(attrJointIDs, 2, GL_INT, 0, NULL);
    }

    // Bind the index buffer and then draw shapes from it.
    // This invokes the shader program, which accesses the vertex buffers.
    d.bindIdx();
    f.glDrawElements(d.drawMode(), d.elemCount(), GL_UNSIGNED_INT, 0);

    if (attrPos         != -1) prog.disableAttributeArray(attrPos);
    if (attrNor         != -1) prog.disableAttributeArray(attrNor);
    if (attrCol         != -1) prog.disableAttributeArray(attrCol);
    if (attrInfluences  != -1) prog.disableAttributeArray(attrInfluences);
    if (attrJointIDs    != -1) prog.disableAttributeArray(attrJointIDs);

    f.printGLErrorLog();
}
