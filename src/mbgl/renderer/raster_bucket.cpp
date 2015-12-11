#include <mbgl/renderer/raster_bucket.hpp>
#include <mbgl/layer/raster_layer.hpp>
#include <mbgl/shader/raster_shader.hpp>
#include <mbgl/renderer/painter.hpp>

using namespace mbgl;

RasterBucket::RasterBucket(TexturePool& texturePool)
: raster(texturePool) {
}

void RasterBucket::upload() {
    if (hasData()) {
        raster.upload();
        uploaded = true;
    }
}

void RasterBucket::render(Painter& painter,
                          const Layer& layer,
                          const TileID& id,
                          const mat4& matrix) {
    painter.renderRaster(*this, *layer.impl->as<RasterLayer>(), id, matrix);
}

bool RasterBucket::setImage(PremultipliedImage image) {
    return raster.load(std::move(image));
}

void RasterBucket::drawRaster(RasterShader& shader, StaticVertexBuffer &vertices, VertexArrayObject &array) {
    raster.bind(true);
    shader.u_image = 0;
    array.bind(shader, vertices, BUFFER_OFFSET_0);
    MBGL_CHECK_ERROR(glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.index()));
}

bool RasterBucket::hasData() const {
    return raster.isLoaded();
}
