#include "catch.hpp"
#include "MetaImageImporter.hpp"
#include "MetaImageStreamer.hpp"
#include "GaussianSmoothingFilter.hpp"
#include "SliceRenderer.hpp"
#include "SurfaceRenderer.hpp"
#include "SimpleWindow.hpp"
#include "DeviceManager.hpp"

using namespace fast;

TEST_CASE("Simple pipeline with MetaImageStreamer, GaussianSmoothingFilter and SliceRenderer on OpenCL device", "[fast][SystemTests]") {
    CHECK_NOTHROW(
    MetaImageStreamer::pointer mhdStreamer = MetaImageStreamer::New();
    mhdStreamer->setFilenameFormat(std::string(FAST_TEST_DATA_DIR)+"/US-3Dt/US-3Dt_#.mhd");
    mhdStreamer->setStreamingMode(STREAMING_MODE_PROCESS_ALL_FRAMES);

    GaussianSmoothingFilter::pointer filter = GaussianSmoothingFilter::New();
    filter->setInput(mhdStreamer->getOutput());
    filter->setMaskSize(5);
    filter->setStandardDeviation(2.0);

    SliceRenderer::pointer renderer = SliceRenderer::New();
    renderer->setInput(filter->getOutput());

    SimpleWindow::pointer window = SimpleWindow::New();
    window->addRenderer(renderer);
    window->setTimeout(10*1000); // timeout after 10 seconds
    window->runMainLoop();
    );
}

TEST_CASE("Simple pipeline with MetaImageStreamer, GaussianSmoothingFilter and SurfaceRenderer on OpenCL device", "[fast][SystemTests]") {
    CHECK_NOTHROW(
    MetaImageStreamer::pointer mhdStreamer = MetaImageStreamer::New();
    mhdStreamer->setFilenameFormat(std::string(FAST_TEST_DATA_DIR)+"/US-3Dt/US-3Dt_#.mhd");
    mhdStreamer->setStreamingMode(STREAMING_MODE_PROCESS_ALL_FRAMES);

    GaussianSmoothingFilter::pointer filter = GaussianSmoothingFilter::New();
    filter->setInput(mhdStreamer->getOutput());
    filter->setMaskSize(5);
    filter->setStandardDeviation(2.0);

    SurfaceRenderer::pointer renderer = SurfaceRenderer::New();
    renderer->setInput(filter->getOutput());
    renderer->setThreshold(200);

    SimpleWindow::pointer window = SimpleWindow::New();
    window->addRenderer(renderer);
    window->setTimeout(10*1000); // timeout after 10 seconds
    window->runMainLoop();
    );
}

TEST_CASE("Simple pipeline with MetaImageStreamer, GaussianSmoothingFilter and SliceRenderer on Host", "[fast][SystemTests]") {
    CHECK_NOTHROW(
    MetaImageStreamer::pointer mhdStreamer = MetaImageStreamer::New();
    mhdStreamer->setFilenameFormat(std::string(FAST_TEST_DATA_DIR)+"/US-3Dt/US-3Dt_#.mhd");
    mhdStreamer->setStreamingMode(STREAMING_MODE_PROCESS_ALL_FRAMES);
    mhdStreamer->setDevice(Host::New());

    GaussianSmoothingFilter::pointer filter = GaussianSmoothingFilter::New();
    filter->setInput(mhdStreamer->getOutput());
    filter->setMaskSize(3);
    filter->setStandardDeviation(2.0);
    filter->setDevice(Host::New());

    SliceRenderer::pointer renderer = SliceRenderer::New();
    renderer->setInput(filter->getOutput());

    SimpleWindow::pointer window = SimpleWindow::New();
    window->addRenderer(renderer);
    window->setTimeout(10*1000); // timeout after 10 seconds
    window->runMainLoop();
    );
}

TEST_CASE("Simple pipeline with MetaImageStreamer, GaussianSmoothingFilter and SurfaceRenderer on Host", "[fast][SystemTests]") {
    CHECK_NOTHROW(
    MetaImageStreamer::pointer mhdStreamer = MetaImageStreamer::New();
    mhdStreamer->setFilenameFormat(std::string(FAST_TEST_DATA_DIR)+"/US-3Dt/US-3Dt_#.mhd");
    mhdStreamer->setStreamingMode(STREAMING_MODE_PROCESS_ALL_FRAMES);
    mhdStreamer->setDevice(Host::New());

    GaussianSmoothingFilter::pointer filter = GaussianSmoothingFilter::New();
    filter->setInput(mhdStreamer->getOutput());
    filter->setMaskSize(3);
    filter->setStandardDeviation(2.0);
    filter->setDevice(Host::New());

    SurfaceRenderer::pointer renderer = SurfaceRenderer::New();
    renderer->setInput(filter->getOutput());
    renderer->setThreshold(200);

    SimpleWindow::pointer window = SimpleWindow::New();
    window->addRenderer(renderer);
    window->setTimeout(10*1000); // timeout after 10 seconds
    window->runMainLoop();
    );
}