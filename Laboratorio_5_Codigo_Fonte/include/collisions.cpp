
glm::vec3 getBoundingBoxMin(const char* object_name){
    // "Ligamos" o VAO. Informamos que queremos utilizar os atributos de
    // vértices apontados pelo VAO criado pela função BuildTrianglesAndAddToVirtualScene(). Veja
    // comentários detalhados dentro da definição de BuildTrianglesAndAddToVirtualScene().
    glBindVertexArray(g_VirtualScene[object_name].vertex_array_object_id);

    // Setamos as variáveis "bbox_min" e "bbox_max" do fragment shader
    // com os parâmetros da axis-aligned bounding box (AABB) do modelo.
    return g_VirtualScene[object_name].bbox_min;
}


glm::vec3 getBoundingBoxMax(const char* object_name){
    // "Ligamos" o VAO. Informamos que queremos utilizar os atributos de
    // vértices apontados pelo VAO criado pela função BuildTrianglesAndAddToVirtualScene(). Veja
    // comentários detalhados dentro da definição de BuildTrianglesAndAddToVirtualScene().
    glBindVertexArray(g_VirtualScene[object_name].vertex_array_object_id);

    // Setamos as variáveis "bbox_min" e "bbox_max" do fragment shader
    // com os parâmetros da axis-aligned bounding box (AABB) do modelo.
    return g_VirtualScene[object_name].bbox_max;
}

// sphere sphere collision
bool ghostCollide(glm::vec4 pacman_position, OBJETO ghost){
    glm::vec3 bbox_min = getBoundingBoxMin("ghost");
    glm::vec3 bbox_max  = getBoundingBoxMax("ghost");
    glm::vec4 ghost_position = glm::vec4(ghost.trans.x, ghost.trans.y, ghost.trans.z, 1);
    float distance = glm::length(pacman_position - ghost_position);
    if(distance < 0.9){
        return true;
    }
    return false;
}

float planeDistance(glm::vec4 point1, glm::vec4 point2, glm::vec4 point3, glm::vec4 pacman_position){
    glm::vec4 plane_normal = crossproduct(point2 - point1, point3 - point1);
    float distance = glm::dot(plane_normal, pacman_position - point1);
    return distance;
}

float pointsDistance(glm::vec4 point1, glm::vec4 point2){
    float distance = glm::length(point1 - point2);
    return distance;
}

glm::vec4 normalizeVector(glm::vec4 vector){
    float length = glm::length(vector);
    glm::vec4 normalized_vector = vector / length;
    return normalized_vector;
}

bool testFaceCollision(glm::vec4 point1, glm::vec4 point2, glm::vec4 point3, glm::vec4 point4, glm::vec4 pacman_position, float distance){
    float distance1 = pointsDistance(point1, pacman_position);
    float distance2 = pointsDistance(point2, pacman_position);
    float distance3 = pointsDistance(point3, pacman_position);
    float distance4 = pointsDistance(point4, pacman_position);

    glm::vec4 point1_2_2 = point1 + (point2 - point1) * 0.5f;
    glm::vec4 point2_3_2 = point2 + (point3 - point2) * 0.5f;
    glm::vec4 point3_4_2 = point3 + (point4 - point3) * 0.5f;
    glm::vec4 point4_1_2 = point4 + (point1 - point4) * 0.5f;

    glm::vec4 midPoint = (point1_2_2 + point2_3_2 + point3_4_2 + point4_1_2) * 0.25f;

    float distanceMid = pointsDistance(midPoint, pacman_position);

    bool collidesWithCorners = distance1 < distance || distance2 < distance || distance3 < distance || distance4 < distance;

    float distanceMid_1 = glm::dot(normalizeVector(point1_2_2), midPoint - pacman_position);
    float distanceMid_2 = glm::dot(normalizeVector(point2_3_2), midPoint - pacman_position);
    float distanceMid_3 = glm::dot(normalizeVector(point3_4_2), midPoint - pacman_position);
    float distanceMid_4 = glm::dot(normalizeVector(point4_1_2), midPoint - pacman_position);

    bool insideBox = distanceMid_1 < glm::length(point1_2_2) && distanceMid_2 < glm::length(point2_3_2) && distanceMid_3 < glm::length(point3_4_2) && distanceMid_4 < glm::length(point4_1_2);

    float pacmanPlaneDistance = planeDistance(point1, point2, point3, pacman_position);

    bool collidesWithPlane = pacmanPlaneDistance < distance && pacmanPlaneDistance > -distance;

    return collidesWithCorners || (insideBox && collidesWithPlane);

}





// colisão Sphere com Square
bool wallCollide(glm::vec4 pacman_position, OBJETO wall){
    return false;
    // Não funcionou :/
    // Mas acho que pode ter sido quase


    glm::vec3 bbox_min_base = getBoundingBoxMin("wall");
    glm::vec4 bbox_min = glm::vec4(bbox_min_base.x, bbox_min_base.y, bbox_min_base.z, 1);

    glm::vec3 bbox_max_base = getBoundingBoxMax("wall");
    glm::vec4 bbox_max = glm::vec4(bbox_max_base.x, bbox_max_base.y, bbox_max_base.z, 1);


    glm::mat4 model =
                 Matrix_Translate(wall.trans.x, wall.trans.y, wall.trans.z)
                * Matrix_Rotate_X(wall.rotat.x) * Matrix_Rotate_Y(wall.rotat.y) * Matrix_Rotate_Z(wall.rotat.z)
                * Matrix_Scale(wall.scal.x, wall.scal.y, wall.scal.z);

    bbox_min = model * bbox_min;
    bbox_max = model * bbox_max;



    glm::vec4 point1 = glm::vec4(bbox_min.x, bbox_min.y, bbox_min.z, 1);
    glm::vec4 point2 = glm::vec4(bbox_max.x, bbox_min.y, bbox_min.z, 1);
    glm::vec4 point3 = glm::vec4(bbox_min.x, bbox_max.y, bbox_min.z, 1);
    glm::vec4 point4 = glm::vec4(bbox_min.x, bbox_min.y, bbox_max.z, 1);

    glm::vec4 point5 = glm::vec4(bbox_max.x, bbox_max.y, bbox_min.z, 1);
    glm::vec4 point6 = glm::vec4(bbox_max.x, bbox_min.y, bbox_max.z, 1);
    glm::vec4 point7 = glm::vec4(bbox_min.x, bbox_max.y, bbox_max.z, 1);
    glm::vec4 point8 = glm::vec4(bbox_max.x, bbox_max.y, bbox_max.z, 1);

    float pacmanSize = 0.5f;
    bool collidesWithFace1 = testFaceCollision(point1, point2, point5, point3, pacman_position, pacmanSize);
    bool collidesWithFace2 = testFaceCollision(point1, point4, point6, point2, pacman_position, pacmanSize);
    bool collidesWithFace3 = testFaceCollision(point1, point3, point7, point4, pacman_position, pacmanSize);
    bool collidesWithFace4 = testFaceCollision(point8, point6, point4, point7, pacman_position, pacmanSize);
    bool collidesWithFace5 = testFaceCollision(point8, point5, point2, point6, pacman_position, pacmanSize);
    bool collidesWithFace6 = testFaceCollision(point8, point7, point3, point5, pacman_position, pacmanSize);

    return collidesWithFace1 || collidesWithFace2 || collidesWithFace3 || collidesWithFace4 || collidesWithFace5 || collidesWithFace6;





    return false;
}


// esfera ponto
bool heartCollide(glm::vec4 pacman_position, OBJETO heart){
    glm::vec3 bbox_min = getBoundingBoxMin("heart");
    glm::vec3 bbox_max = getBoundingBoxMax("heart");
    glm::vec4 heartPosition = glm::vec4(heart.trans.x, heart.trans.y, heart.trans.z, 1);

    // a esfera tem um raio de captura ao redor dela, então se o coração estiver dentro desse raio, ele é capturado
    float captureRadius = 0.2;
    float pacmanScale = 0.4;
    float pacmanRadius = abs(bbox_max.x - bbox_min.x) * pacmanScale / 2; // /2 due to diameter to radius
    float finalSphereRadius =  pacmanRadius + captureRadius;
    float distance = glm::length(pacman_position - heartPosition);
    if(distance < finalSphereRadius){
        return true;
    }
    return false;
}


