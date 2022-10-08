
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

bool wallCollide(glm::vec4 pacman_position, OBJETO wall){
    glm::vec3 bbox_min = getBoundingBoxMin("wall");
    glm::vec3 bbox_max = getBoundingBoxMax("wall");
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


