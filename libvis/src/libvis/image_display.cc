// Copyright 2018 ETH Zürich, Thomas Schöps
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.


#include "libvis/image_display.h"

namespace vis {

ImageDisplay::ImageDisplay()
    : window_(nullptr) {
  // Ensure that the Qt thread is running.
  QtThread::Instance()->WaitForStartup();
  
  // Create all QObjects in the Qt thread.
  QtThread::Instance()->RunInQtThreadBlocking([&](){
    window_ = new ImageDisplayQtWindow(this);
  });
}

ImageDisplay::~ImageDisplay() {
  QtThread::Instance()->RunInQtThreadBlocking([&](){
    if (window_) {
      window_->SetDisplay(nullptr);
    }
  });
}

void ImageDisplay::Clear() {
  QtThread::Instance()->RunInQtThreadBlocking([&](){
    window_->Clear();
  });
}

void ImageDisplay::Close() {
  QtThread::Instance()->RunInQtThreadBlocking([&](){
    delete window_;
    window_ = nullptr;
  });
}

void ImageDisplay::SetWindow(ImageDisplayQtWindow* window) {
  window_ = window;
}

bool ImageDisplay::IsOpen() {
  bool result = false;
  QtThread::Instance()->RunInQtThreadBlocking([&](){
    if (window_ && window_->isVisible()) {
      result = true;
    }
  });
  return result;
}

}
