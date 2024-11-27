#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <util.h>

#include <transform.hpp>
#include <camera.hpp>
#include <model.hpp>
#include <object.hpp>
/**
 * all headers about gls.
 * transform: transforming of models
 * model: loading and drawing models
 * object: combine of models, texture, and transform.
 * camera: camera control and perspective control.
 * texture: TODO, loading and updating textures.
 * 
 * Should use:
 *  create a model pool to store the model, texture pool to store the texture
 *  create objects
 *  create camera
 *  loop:
 *      read input
 *      setting object transform
 *      setting camera
 *      render
 *  destroy objects
 *  destroy models(models free space in GPU)
 *  destroy textures(textures free space in GPU)
*/