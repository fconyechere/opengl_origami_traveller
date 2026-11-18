#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Origami girl rig. Every part OBJ has its joint pivot at the local origin (0,0,0),
// so rotating a part about its OBJ origin = rotating about the joint.
// Model space: +Y up, facing +Z, height 1.0. All joints are children of the torso.
// At rest (all rotations = identity, torso at origin) the parts reassemble the original T-pose.

// Torso origin = midpoint between the hip joints (in the original OBJ space this is at (-0.016045f, 0.311500f, -0.019308f))
static const glm::vec3 JOINT_HEAD(0.003922f, 0.464500f, 0.002610f);  // offset from torso origin
static const glm::vec3 JOINT_ARM_L(0.148545f, 0.393708f, -0.000657f);  // offset from torso origin
static const glm::vec3 JOINT_ARM_R(-0.136455f, 0.387345f, -0.000598f);  // offset from torso origin
static const glm::vec3 JOINT_LEG_L(0.041295f, 0.000000f, -0.000014f);  // offset from torso origin
static const glm::vec3 JOINT_LEG_R(-0.041295f, 0.000000f, 0.000014f);  // offset from torso origin

struct Pose {                 // Euler angles in radians, per joint
    glm::vec3 torso{0}, head{0}, armL{0}, armR{0}, legL{0}, legR{0};
    glm::vec3 torsoPos{0};
};
static inline glm::mat4 rot(const glm::vec3& r){
    glm::mat4 m(1.0f);
    m = glm::rotate(m, r.y, glm::vec3(0,1,0));
    m = glm::rotate(m, r.x, glm::vec3(1,0,0));
    m = glm::rotate(m, r.z, glm::vec3(0,0,1));
    return m;
}
// Fill the 6 model matrices (pass each one to the shader before drawing that part's mesh).
static inline void buildModelMatrices(const Pose& p, const glm::mat4& world, glm::mat4 out[6]){
    glm::mat4 torso = world * glm::translate(glm::mat4(1.0f), p.torsoPos) * rot(p.torso);
    out[0] = torso;                                                                        // torso
    out[1] = torso * glm::translate(glm::mat4(1.0f), JOINT_HEAD)  * rot(p.head);           // head
    out[2] = torso * glm::translate(glm::mat4(1.0f), JOINT_ARM_L) * rot(p.armL);           // arm_L
    out[3] = torso * glm::translate(glm::mat4(1.0f), JOINT_ARM_R) * rot(p.armR);           // arm_R
    out[4] = torso * glm::translate(glm::mat4(1.0f), JOINT_LEG_L) * rot(p.legL);           // leg_L
    out[5] = torso * glm::translate(glm::mat4(1.0f), JOINT_LEG_R) * rot(p.legR);           // leg_R
}
