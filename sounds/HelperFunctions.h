#pragma once
extern "C" {
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}
#include <AL/alc.h>
#include <AL/alext.h>

extern LPALGENEFFECTS myAlGenEffects;
extern LPALGENAUXILIARYEFFECTSLOTS myAlGenAuxiliaryEffectSlots;
extern LPALAUXILIARYEFFECTSLOTI myAlAuxiliaryEffectSloti;
extern LPALEFFECTI myAlEffecti;
extern LPALISEFFECT myAlIsEffect;
extern LPALDELETEEFFECTS myAlDeleteEffects;
auto getALFormat(AVSampleFormat AVFormat, int channels) -> ALenum;
auto setSampleFormat(AVCodecContext& context, const AVCodec& codec) -> AVSampleFormat;
void deleteBuffer(ALuint* buffer);
void deletePacket(AVPacket* packet);
void deleteFrame(AVFrame* frame);
auto decodeFile(AVFormatContext& formatContext, const AVStream& audioStream, AVCodecContext& codecContext) -> std::vector<unsigned char>;
auto getALDevice() -> ALCdevice*;
auto getALContext() -> ALCcontext*;