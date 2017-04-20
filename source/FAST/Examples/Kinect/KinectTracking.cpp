#include <FAST/Streamers/KinectStreamer.hpp>
#include "KinectTracking.hpp"
#include "FAST/Data/Image.hpp"
#include "FAST/Data/Mesh.hpp"

namespace fast {

KinectTracking::KinectTracking() {

    createInputPort<Image>(0);
    createInputPort<Mesh>(1);

    createOutputPort<Image>(0, OUTPUT_DEPENDS_ON_INPUT, 0);
    createOutputPort<Image>(1, OUTPUT_STATIC); // Annotation image
    //createOutputPort<Mesh>(2, OUTPUT_STATIC); // Target cloud

    // Create annotation image
    mAnnotationImage = Image::New();
    mAnnotationImage->create(512, 424, TYPE_UINT8, 1);
    mAnnotationImage->fill(0);
}

void KinectTracking::execute() {
    Image::pointer input = getStaticInputData<Image>();
    Mesh::pointer meshInput = getStaticInputData<Mesh>(1);

    setStaticOutputData<Image>(0, input);
    setStaticOutputData<Image>(1, mAnnotationImage);
    mCurrentCloud = meshInput;
}

Mesh::pointer KinectTracking::getTargetCloud(KinectStreamer::pointer streamer) {
    std::cout << "Creating target cloud..." << std::endl;
    ImageAccess::pointer access = mAnnotationImage->getImageAccess(ACCESS_READ);
    MeshAccess::pointer meshAccess = mCurrentCloud->getMeshAccess(ACCESS_READ);
    std::vector<MeshVertex> vertices = meshAccess->getVertices();
    std::vector<MeshVertex> outputVertices;
    for(int y = 0; y < mAnnotationImage->getHeight(); ++y) {
        for(int x = 0; x < mAnnotationImage->getWidth(); ++x) {
            try {
                if(access->getScalar(Vector2i(x, y)) == 1) {
                    MeshVertex vertex = streamer->getPoint(x, y);
                    if(!std::isnan(vertex.getPosition().x())) {
                        outputVertices.push_back(vertex);
                    }
                }
            } catch (Exception &e) {

            }
        }
    }

    Mesh::pointer output = Mesh::New();
    output->create(outputVertices);
    std::cout << "Created target cloud." << std::endl;
    return output;
}

void KinectTracking::addLine(Vector2i start, Vector2i end) {
    std::cout << "Drawing from: " << start.transpose() << " to " << end.transpose() << std::endl;
    // Draw line in some auxillary image
    ImageAccess::pointer access = mAnnotationImage->getImageAccess(ACCESS_READ_WRITE);
    Vector2f direction = end.cast<float>() - start.cast<float>();
    int length = (end-start).norm();
    int brushSize = 6;
    for(int i = 0; i < length; ++i) {
        float distance = (float)i/length;
        for(int a = -brushSize; a <= brushSize; a++) {
            for(int b = -brushSize; b <= brushSize; b++) {
                Vector2f offset(a, b);
                if(offset.norm() > brushSize)
                    continue;
                Vector2f position = start.cast<float>() + direction*distance + offset;
                try {
                    access->setScalar(position.cast<int>(), 1);
                } catch(Exception &e) {

                }
            }
        }
    }
}

}