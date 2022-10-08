#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

in float lambert_v;
// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Identificador que define qual objeto está sendo desenhado no momento
#define SPHERE 0
#define GHOST  1
#define HEART  2
#define PLANE  3
#define FLOOR  4
uniform int object_id;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

#define PHONG 0
#define GOURAUD 1
uniform int interpolation_mode;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;
uniform sampler2D TextureImage4;


uniform vec3 scale_proportion;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

float func_mod(float x, float y) {
    return x-y*floor(x/y);
}


void main()
{
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(vec4(1.0,1.0,1.0,0.0));

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;


    // Obtemos a refletância difusa a partir da leitura da imagem TextureImage0
    vec3 Kd0 = texture(TextureImage0, vec2(U,V)).rgb;

    if ( object_id == SPHERE )
    {
        // PREENCHA AQUI as coordenadas de textura da esfera, computadas com
        // projeção esférica EM COORDENADAS DO MODELO. Utilize como referência
        // o slides 134-150 do documento Aula_20_Mapeamento_de_Texturas.pdf.
        // A esfera que define a projeção deve estar centrada na posição
        // "bbox_center" definida abaixo.

        // Você deve utilizar:
        //   função 'length( )' : comprimento Euclidiano de um vetor
        //   função 'atan( , )' : arcotangente. Veja https://en.wikipedia.org/wiki/Atan2.
        //   função 'asin( )'   : seno inverso.
        //   constante M_PI
        //   variável position_model

        vec4 bbox_center = (bbox_min + bbox_max) / 2.0;
        vec4 c = bbox_center;

        vec4 pp = c + normalize(position_model - c);
        pp = pp - c;

        float tetha = atan(pp.x, pp.z);
        float phi = asin(pp.y);

        U = (tetha + M_PI) / (2*M_PI);
        V = (phi + (M_PI/2)) / M_PI;

        Kd0 = texture(TextureImage0 , vec2(U,V)).rgb;


    }
    else if ( object_id == GHOST )
    {
        // Coordenadas de textura do fantasma, obtidas do arquivo OBJ.


        vec4 bbox_center = (bbox_min + bbox_max) / 2.0;
        vec4 c = bbox_center;

        vec4 pp = c + normalize(position_model - c);
        pp = pp - c;

        float tetha = atan(pp.x, pp.z);
        float phi = asin(pp.y);

        U = (tetha + M_PI) / (2*M_PI);
        V = (phi + (M_PI/2)) / M_PI;

        Kd0 = texture(TextureImage1 , vec2(U,V)).rgb;

    }
    else if ( object_id == HEART )
    {
        // PREENCHA AQUI as coordenadas de textura do coelho, computadas com
        // projeção planar XY em COORDENADAS DO MODELO. Utilize como referência
        // o slides 99-104 do documento Aula_20_Mapeamento_de_Texturas.pdf,
        // e também use as variáveis min*/max* definidas abaixo para normalizar
        // as coordenadas de textura U e V dentro do intervalo [0,1]. Para
        // tanto, veja por exemplo o mapeamento da variável 'p_v' utilizando
        // 'h' no slides 158-160 do documento Aula_20_Mapeamento_de_Texturas.pdf.
        // Veja também a Questão 4 do Questionário 4 no Moodle.

        float minx = bbox_min.x;
        float maxx = bbox_max.x;

        float miny = bbox_min.y;
        float maxy = bbox_max.y;

        float minz = bbox_min.z;
        float maxz = bbox_max.z;

        float px = (position_model.x - minx) / (maxx - minx);
        float py = (position_model.y - miny) / (maxy - miny);

        U = px;
        V = py;

        Kd0 = texture(TextureImage2, vec2(U,V)).rgb;

    }
    else if ( object_id == PLANE  || object_id == FLOOR )
    {
        // PREENCHA AQUI as coordenadas de textura do coelho, computadas com
        // projeção planar XY em COORDENADAS DO MODELO. Utilize como referência
        // o slides 99-104 do documento Aula_20_Mapeamento_de_Texturas.pdf,
        // e também use as variáveis min*/max* definidas abaixo para normalizar
        // as coordenadas de textura U e V dentro do intervalo [0,1]. Para
        // tanto, veja por exemplo o mapeamento da variável 'p_v' utilizando
        // 'h' no slides 158-160 do documento Aula_20_Mapeamento_de_Texturas.pdf.
        // Veja também a Questão 4 do Questionário 4 no Moodle.

        int scale = 20;
        int loop = 1;

        float scale_props = scale_proportion.x / scale_proportion.z;
        float z_scale_scale = scale / scale_proportion.z;


        float px = position_model.x * scale * scale_props / z_scale_scale;
        float pz = position_model.z * scale / z_scale_scale;
        //float px = func_mod(position_model.x, stone_texture_x) / stone_texture_x;
        //float pz = func_mod(position_model.z, stone_texture_y) / stone_texture_y;

        U = px;
        V = pz;

        if(object_id == PLANE)
            Kd0 = texture(TextureImage3, vec2(U,V)).rgb;
        else
            Kd0 = texture(TextureImage4, vec2(U,V)).rgb;
    }



    // Equação de Iluminação
    float lambert = max(0,dot(n,l));
    float inverted_lambert = max(0,dot(-n,l));

    if(interpolation_mode == GOURAUD) {
        lambert = max(0,lambert_v);
        inverted_lambert = max(0, -lambert_v);
    }

    // Equação de Iluminação

    vec3 light_color = vec3(1.0f,1.0f,1.0f);

    float ambient_light_const = 0.2;


    color.rgb = (Kd0 * min(lambert + ambient_light_const, 1));

    if(object_id == HEART || object_id == SPHERE) {
        vec4 r = -l + 2 * n * (dot(n,l));
        vec4 v_normal = v/length(v);
        vec3 ks = vec3(1,1,1);
        int q = 40;
        int ql = 60;

        bool blinn_phong = true;

        if(blinn_phong){
            vec4 h = (v+l) / length(v+l);
            float positive_beta = max(0, dot(n,h));
            color.rgb += ks*light_color*pow(positive_beta,ql);
        } else {
            float positive_rv = max(0, dot(r,v));
            color.rgb += ks*light_color*pow(positive_rv,q);
        }
    }

    // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
    // necessário:
    // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
    //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
    //      glEnable(GL_BLEND);
    //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
    //    todos os objetos opacos; e
    // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
    //    suas distâncias para a câmera (desenhando primeiro objetos
    //    transparentes que estão mais longe da câmera).
    // Alpha default = 1 = 100% opaco = 0% transparente
    color.a = 1;

    // Cor final com correção gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
}
