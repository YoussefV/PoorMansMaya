#ifndef LA
#define LA
#define GLM_CIS277  // Don't copy this include!
// Primary GLM library
#    include <glm/glm.hpp>
// For glm::translate, glm::rotate, and glm::scale.
#    include <glm/gtc/matrix_transform.hpp>
// For glm::to_string.
#    include <glm/gtx/string_cast.hpp>
// For glm::value_ptr.
#    include <glm/gtc/type_ptr.hpp>
//#undef GLM_CIS277

#include <QMatrix4x4>
#include<QVector4D>

/// 277 linear algebra namespace
namespace la {
    QMatrix4x4 to_qmat(const glm::mat4 &m);
    QVector4D to_qvec(const glm::vec4 &v);
}


#endif // LA
