/**
 * @file
 *
 * @section LICENSE
 *
 * OsmAnd - Android navigation software based on OSM maps.
 * Copyright (C) 2010-2013  OsmAnd Authors listed in AUTHORS file
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __ATLAS_MAP_RENDERER_OPENGL_COMMON_H_
#define __ATLAS_MAP_RENDERER_OPENGL_COMMON_H_

#include <stdint.h>
#include <memory>

#include <glm/glm.hpp>

#include <OsmAndCore.h>
#include <OsmAndCore/CommonTypes.h>
#include <AtlasMapRenderer.h>
#include <OpenGL_Common/RenderAPI_OpenGL_Common.h>

namespace OsmAnd {

    class OSMAND_CORE_API AtlasMapRenderer_OpenGL_Common : public AtlasMapRenderer
    {
    public:
    private:
    protected:
        AtlasMapRenderer_OpenGL_Common();

        glm::mat4 _mProjection;
        glm::mat4 _mProjectionInv;
        glm::mat4 _mView;
        glm::mat4 _mDistance;
        glm::mat4 _mElevation;
        glm::mat4 _mAzimuth;
        glm::mat4 _mViewInv;
        glm::mat4 _mDistanceInv;
        glm::mat4 _mElevationInv;
        glm::mat4 _mAzimuthInv;
        const float _zNear;
        float _zSkyplane;
        float _zFar;
        float _projectionPlaneHalfHeight;
        float _projectionPlaneHalfWidth;
        float _aspectRatio;
        float _fovInRadians;
        float _nearDistanceFromCameraToTarget;
        float _baseDistanceFromCameraToTarget;
        float _farDistanceFromCameraToTarget;
        float _distanceFromCameraToTarget;
        float _groundDistanceFromCameraToTarget;
        float _tileScaleFactor;
        float _scaleToRetainProjectedSize;
        PointF _skyplaneHalfSize;
        float _correctedFogDistance;
        float _mipmapK;
        
        void computeVisibleTileset();

#pragma pack(push)
#pragma pack(1)
        struct MapTileVertex 
        {
            GLfloat position[2];
            GLfloat uv[2];
        };
#pragma pack(pop)

        virtual void validateConfigurationChange(const ConfigurationChange& change);

        GLsizei _tilePatchIndicesCount;
        virtual void createTilePatch();
        virtual void allocateTilePatch(MapTileVertex* vertices, GLsizei verticesCount, GLushort* indices, GLsizei indicesCount) = 0;
        virtual void releaseTilePatch() = 0;
        
        virtual void validateLayer(const MapTileLayerId& layer);

        virtual bool updateCurrentState();

        virtual bool postInitializeRendering();
        virtual bool preReleaseRendering();

        RenderAPI_OpenGL_Common* getRenderAPI() const;

        virtual std::shared_ptr<IMapTileProvider::Tile> prepareTileForUploadingToGPU(const std::shared_ptr<IMapTileProvider::Tile>& tile);
    public:
        virtual ~AtlasMapRenderer_OpenGL_Common();

        virtual float getReferenceTileSizeOnScreen();
        virtual float getScaledTileSizeOnScreen();
    };

}

#endif // __ATLAS_MAP_RENDERER_OPENGL_COMMON_H_
