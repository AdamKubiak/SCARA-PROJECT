#include "render_object.h"

Render_Object::Render_Object(Qt3DCore::QEntity *parent)
    : Qt3DCore::QEntity(parent)
    , local_transform(new Qt3DCore::QTransform()),local_loader(new Qt3DRender::QSceneLoader(parent))
{
    addComponent(local_transform);
    addComponent(local_loader);
    qDebug()<<parent;
}


Qt3DCore::QTransform *Render_Object::transform()
{
    return local_transform;
}

Qt3DRender::QTextureImage *Render_Object::texture()
{
    return local_texture;
}

Qt3DExtras::QDiffuseSpecularMapMaterial *Render_Object::map()
{
    return local_map;
}

Qt3DRender::QMaterial *Render_Object::material()
{
    return local_material;
}

Qt3DExtras::QTorusMesh *Render_Object::torusMesh()
{
    return local_torusMesh;
}

Qt3DRender::QSceneLoader *Render_Object::loader()
{
    return local_loader;
}

Qt3DRender::QMesh *Render_Object::mesh()
{
    return local_mesh;
}



