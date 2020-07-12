#include "Model.h"

unsigned int Model::constructed_model_count{0};

void Model::assignID(){
    render_id = constructed_model_count++;
}

Model::Model()
{
    assignID();
}

