#ifndef MISSILE_WEAPON_W_SVG_H
#define MISSILE_WEAPON_W_SVG_H

#include <QSvgRenderer>
#include <QPainter>
#include <QImage>

class MISSILE_WEAPON_W_SVG {
 private:
  uchar data_[2250] = {
    0x3c,0x3f,0x78,0x6d,0x6c,0x20,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x3d,0x22,0x31,
    0x2e,0x30,0x22,0x20,0x65,0x6e,0x63,0x6f,0x64,0x69,0x6e,0x67,0x3d,0x22,0x55,0x54,
    0x46,0x2d,0x38,0x22,0x20,0x73,0x74,0x61,0x6e,0x64,0x61,0x6c,0x6f,0x6e,0x65,0x3d,
    0x22,0x6e,0x6f,0x22,0x3f,0x3e,0x0a,0x3c,0x73,0x76,0x67,0x0a,0x20,0x20,0x20,0x77,
    0x69,0x64,0x74,0x68,0x3d,0x22,0x31,0x39,0x32,0x6d,0x6d,0x22,0x0a,0x20,0x20,0x20,
    0x68,0x65,0x69,0x67,0x68,0x74,0x3d,0x22,0x31,0x39,0x32,0x6d,0x6d,0x22,0x0a,0x20,
    0x20,0x20,0x76,0x69,0x65,0x77,0x42,0x6f,0x78,0x3d,0x22,0x30,0x20,0x30,0x20,0x31,
    0x39,0x32,0x20,0x31,0x39,0x32,0x22,0x0a,0x20,0x20,0x20,0x76,0x65,0x72,0x73,0x69,
    0x6f,0x6e,0x3d,0x22,0x31,0x2e,0x31,0x22,0x0a,0x20,0x20,0x20,0x69,0x64,0x3d,0x22,
    0x73,0x76,0x67,0x35,0x22,0x0a,0x20,0x20,0x20,0x78,0x6d,0x6c,0x6e,0x73,0x3d,0x22,
    0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x77,0x77,0x77,0x2e,0x77,0x33,0x2e,0x6f,0x72,
    0x67,0x2f,0x32,0x30,0x30,0x30,0x2f,0x73,0x76,0x67,0x22,0x3e,0x0a,0x20,0x20,0x3c,
    0x64,0x65,0x66,0x73,0x0a,0x20,0x20,0x20,0x20,0x20,0x69,0x64,0x3d,0x22,0x64,0x65,
    0x66,0x73,0x32,0x22,0x2f,0x3e,0x0a,0x20,0x20,0x3c,0x67,0x0a,0x20,0x20,0x20,0x20,
    0x20,0x69,0x64,0x3d,0x22,0x6c,0x61,0x79,0x65,0x72,0x31,0x22,0x3e,0x0a,0x20,0x20,
    0x20,0x20,0x3c,0x67,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x69,0x64,0x3d,0x22,
    0x67,0x39,0x32,0x33,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x74,0x72,0x61,
    0x6e,0x73,0x66,0x6f,0x72,0x6d,0x3d,0x22,0x72,0x6f,0x74,0x61,0x74,0x65,0x28,0x32,
    0x37,0x30,0x2c,0x39,0x36,0x2c,0x39,0x36,0x29,0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,
    0x20,0x20,0x3c,0x63,0x69,0x72,0x63,0x6c,0x65,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x69,0x6c,0x6c,0x3a,0x23,
    0x38,0x30,0x38,0x30,0x38,0x30,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x77,0x69,
    0x64,0x74,0x68,0x3a,0x30,0x2e,0x32,0x33,0x31,0x30,0x32,0x39,0x3b,0x2d,0x69,0x6e,
    0x6b,0x73,0x63,0x61,0x70,0x65,0x2d,0x73,0x74,0x72,0x6f,0x6b,0x65,0x3a,0x6e,0x6f,
    0x6e,0x65,0x3b,0x73,0x74,0x6f,0x70,0x2d,0x63,0x6f,0x6c,0x6f,0x72,0x3a,0x23,0x30,
    0x30,0x30,0x30,0x30,0x30,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x69,0x64,0x3d,0x22,0x70,0x61,0x74,0x68,0x39,0x32,0x36,0x22,0x0a,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x63,0x78,0x3d,0x22,0x39,0x36,0x22,0x0a,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x63,0x79,0x3d,0x22,0x39,0x36,0x22,0x0a,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x72,0x3d,0x22,0x36,0x34,0x22,0x2f,0x3e,
    0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x67,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x69,0x64,0x3d,0x22,0x67,0x33,0x31,0x34,0x38,0x22,0x0a,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x74,0x72,0x61,0x6e,0x73,0x66,0x6f,0x72,0x6d,
    0x3d,0x22,0x6d,0x61,0x74,0x72,0x69,0x78,0x28,0x30,0x2e,0x36,0x35,0x38,0x34,0x37,
    0x39,0x33,0x36,0x2c,0x2d,0x30,0x2e,0x35,0x37,0x39,0x33,0x37,0x35,0x31,0x32,0x2c,
    0x30,0x2e,0x37,0x35,0x32,0x35,0x39,0x38,0x37,0x39,0x2c,0x30,0x2e,0x35,0x30,0x36,
    0x39,0x31,0x38,0x39,0x31,0x2c,0x2d,0x33,0x39,0x2e,0x36,0x39,0x38,0x34,0x37,0x31,
    0x2c,0x38,0x30,0x2e,0x31,0x39,0x33,0x36,0x39,0x31,0x29,0x22,0x3e,0x0a,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x65,0x6c,0x6c,0x69,0x70,0x73,0x65,0x0a,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,
    0x22,0x66,0x69,0x6c,0x6c,0x3a,0x23,0x36,0x63,0x36,0x37,0x35,0x33,0x3b,0x73,0x74,
    0x72,0x6f,0x6b,0x65,0x2d,0x77,0x69,0x64,0x74,0x68,0x3a,0x34,0x30,0x2e,0x31,0x39,
    0x34,0x35,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x6c,0x69,0x6e,0x65,0x63,0x61,
    0x70,0x3a,0x73,0x71,0x75,0x61,0x72,0x65,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,
    0x64,0x61,0x73,0x68,0x6f,0x66,0x66,0x73,0x65,0x74,0x3a,0x32,0x39,0x39,0x2e,0x39,
    0x37,0x37,0x3b,0x2d,0x69,0x6e,0x6b,0x73,0x63,0x61,0x70,0x65,0x2d,0x73,0x74,0x72,
    0x6f,0x6b,0x65,0x3a,0x6e,0x6f,0x6e,0x65,0x3b,0x73,0x74,0x6f,0x70,0x2d,0x63,0x6f,
    0x6c,0x6f,0x72,0x3a,0x23,0x30,0x30,0x30,0x30,0x30,0x30,0x22,0x0a,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x69,0x64,0x3d,0x22,0x70,0x61,0x74,0x68,
    0x32,0x37,0x36,0x39,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x63,0x78,0x3d,0x22,0x37,0x35,0x2e,0x37,0x34,0x32,0x34,0x33,0x32,0x22,0x0a,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x63,0x79,0x3d,0x22,0x31,
    0x33,0x36,0x2e,0x36,0x34,0x32,0x30,0x39,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x72,0x78,0x3d,0x22,0x32,0x37,0x2e,0x36,0x38,0x37,0x33,
    0x34,0x32,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x72,
    0x79,0x3d,0x22,0x32,0x35,0x2e,0x34,0x32,0x35,0x39,0x32,0x36,0x22,0x0a,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x74,0x72,0x61,0x6e,0x73,0x66,0x6f,
    0x72,0x6d,0x3d,0x22,0x6d,0x61,0x74,0x72,0x69,0x78,0x28,0x30,0x2e,0x37,0x33,0x39,
    0x34,0x37,0x32,0x33,0x32,0x2c,0x2d,0x30,0x2e,0x36,0x37,0x33,0x31,0x38,0x36,0x39,
    0x37,0x2c,0x30,0x2e,0x36,0x34,0x34,0x36,0x36,0x35,0x38,0x34,0x2c,0x30,0x2e,0x37,
    0x36,0x34,0x34,0x36,0x34,0x34,0x39,0x2c,0x30,0x2c,0x30,0x29,0x22,0x2f,0x3e,0x0a,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x72,0x65,0x63,0x74,0x0a,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,
    0x66,0x69,0x6c,0x6c,0x3a,0x23,0x66,0x66,0x65,0x65,0x61,0x61,0x3b,0x73,0x74,0x72,
    0x6f,0x6b,0x65,0x2d,0x77,0x69,0x64,0x74,0x68,0x3a,0x34,0x30,0x2e,0x31,0x39,0x34,
    0x35,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x6c,0x69,0x6e,0x65,0x63,0x61,0x70,
    0x3a,0x73,0x71,0x75,0x61,0x72,0x65,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x64,
    0x61,0x73,0x68,0x6f,0x66,0x66,0x73,0x65,0x74,0x3a,0x32,0x39,0x39,0x2e,0x39,0x37,
    0x37,0x3b,0x2d,0x69,0x6e,0x6b,0x73,0x63,0x61,0x70,0x65,0x2d,0x73,0x74,0x72,0x6f,
    0x6b,0x65,0x3a,0x6e,0x6f,0x6e,0x65,0x3b,0x73,0x74,0x6f,0x70,0x2d,0x63,0x6f,0x6c,
    0x6f,0x72,0x3a,0x23,0x30,0x30,0x30,0x30,0x30,0x30,0x22,0x0a,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x69,0x64,0x3d,0x22,0x72,0x65,0x63,0x74,0x32,
    0x37,0x34,0x33,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x77,0x69,0x64,0x74,0x68,0x3d,0x22,0x31,0x34,0x30,0x2e,0x36,0x37,0x35,0x31,0x33,
    0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x68,0x65,0x69,
    0x67,0x68,0x74,0x3d,0x22,0x34,0x39,0x2e,0x35,0x38,0x36,0x37,0x31,0x32,0x22,0x0a,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x78,0x3d,0x22,0x2d,0x38,
    0x35,0x2e,0x33,0x37,0x38,0x37,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x79,0x3d,0x22,0x31,0x31,0x31,0x2e,0x33,0x33,0x39,0x35,0x37,0x22,
    0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x74,0x72,0x61,0x6e,
    0x73,0x66,0x6f,0x72,0x6d,0x3d,0x22,0x6d,0x61,0x74,0x72,0x69,0x78,0x28,0x30,0x2e,
    0x37,0x33,0x39,0x34,0x37,0x32,0x33,0x32,0x2c,0x2d,0x30,0x2e,0x36,0x37,0x33,0x31,
    0x38,0x36,0x39,0x37,0x2c,0x30,0x2e,0x36,0x34,0x34,0x36,0x36,0x35,0x38,0x34,0x2c,
    0x30,0x2e,0x37,0x36,0x34,0x34,0x36,0x34,0x34,0x39,0x2c,0x30,0x2c,0x30,0x29,0x22,
    0x2f,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x72,0x65,0x63,0x74,
    0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x73,0x74,0x79,0x6c,
    0x65,0x3d,0x22,0x66,0x69,0x6c,0x6c,0x3a,0x23,0x61,0x61,0x34,0x34,0x30,0x30,0x3b,
    0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x77,0x69,0x64,0x74,0x68,0x3a,0x34,0x30,0x2e,
    0x31,0x39,0x34,0x35,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x6c,0x69,0x6e,0x65,
    0x63,0x61,0x70,0x3a,0x73,0x71,0x75,0x61,0x72,0x65,0x3b,0x73,0x74,0x72,0x6f,0x6b,
    0x65,0x2d,0x64,0x61,0x73,0x68,0x6f,0x66,0x66,0x73,0x65,0x74,0x3a,0x32,0x39,0x39,
    0x2e,0x39,0x37,0x37,0x3b,0x2d,0x69,0x6e,0x6b,0x73,0x63,0x61,0x70,0x65,0x2d,0x73,
    0x74,0x72,0x6f,0x6b,0x65,0x3a,0x6e,0x6f,0x6e,0x65,0x3b,0x73,0x74,0x6f,0x70,0x2d,
    0x63,0x6f,0x6c,0x6f,0x72,0x3a,0x23,0x30,0x30,0x30,0x30,0x30,0x30,0x22,0x0a,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x69,0x64,0x3d,0x22,0x72,0x65,
    0x63,0x74,0x32,0x37,0x34,0x35,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x77,0x69,0x64,0x74,0x68,0x3d,0x22,0x31,0x39,0x2e,0x36,0x32,0x37,
    0x36,0x33,0x32,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x68,0x65,0x69,0x67,0x68,0x74,0x3d,0x22,0x34,0x39,0x2e,0x36,0x30,0x37,0x37,0x39,
    0x36,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x78,0x3d,
    0x22,0x35,0x35,0x2e,0x32,0x39,0x36,0x33,0x39,0x34,0x22,0x0a,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x79,0x3d,0x22,0x31,0x31,0x31,0x2e,0x33,0x33,
    0x39,0x35,0x37,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x74,0x72,0x61,0x6e,0x73,0x66,0x6f,0x72,0x6d,0x3d,0x22,0x6d,0x61,0x74,0x72,0x69,
    0x78,0x28,0x30,0x2e,0x37,0x33,0x39,0x34,0x37,0x32,0x33,0x32,0x2c,0x2d,0x30,0x2e,
    0x36,0x37,0x33,0x31,0x38,0x36,0x39,0x37,0x2c,0x30,0x2e,0x36,0x34,0x34,0x36,0x36,
    0x35,0x38,0x34,0x2c,0x30,0x2e,0x37,0x36,0x34,0x34,0x36,0x34,0x34,0x39,0x2c,0x30,
    0x2c,0x30,0x29,0x22,0x2f,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,
    0x70,0x61,0x74,0x68,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x69,0x6c,0x6c,0x3a,0x23,0x38,0x30,0x38,
    0x30,0x30,0x30,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x3a,0x23,0x30,0x30,0x30,0x30,
    0x30,0x30,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x77,0x69,0x64,0x74,0x68,0x3a,
    0x30,0x2e,0x32,0x36,0x34,0x35,0x38,0x33,0x70,0x78,0x3b,0x73,0x74,0x72,0x6f,0x6b,
    0x65,0x2d,0x6c,0x69,0x6e,0x65,0x63,0x61,0x70,0x3a,0x62,0x75,0x74,0x74,0x3b,0x73,
    0x74,0x72,0x6f,0x6b,0x65,0x2d,0x6c,0x69,0x6e,0x65,0x6a,0x6f,0x69,0x6e,0x3a,0x6d,
    0x69,0x74,0x65,0x72,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x6f,0x70,0x61,0x63,
    0x69,0x74,0x79,0x3a,0x31,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x64,0x3d,0x22,0x6d,0x20,0x34,0x2e,0x36,0x38,0x30,0x39,0x38,0x37,0x37,
    0x2c,0x31,0x33,0x31,0x2e,0x39,0x35,0x35,0x31,0x32,0x20,0x39,0x2e,0x31,0x36,0x36,
    0x30,0x37,0x35,0x33,0x2c,0x31,0x36,0x2e,0x35,0x32,0x34,0x35,0x39,0x20,0x34,0x36,
    0x2e,0x39,0x38,0x32,0x35,0x34,0x38,0x2c,0x2d,0x34,0x33,0x2e,0x35,0x39,0x35,0x30,
    0x37,0x20,0x7a,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x69,0x64,0x3d,0x22,0x70,0x61,0x74,0x68,0x33,0x30,0x32,0x36,0x22,0x2f,0x3e,0x0a,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x70,0x61,0x74,0x68,0x0a,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,
    0x66,0x69,0x6c,0x6c,0x3a,0x23,0x38,0x30,0x38,0x30,0x30,0x30,0x3b,0x73,0x74,0x72,
    0x6f,0x6b,0x65,0x3a,0x23,0x30,0x30,0x30,0x30,0x30,0x30,0x3b,0x73,0x74,0x72,0x6f,
    0x6b,0x65,0x2d,0x77,0x69,0x64,0x74,0x68,0x3a,0x30,0x2e,0x32,0x36,0x34,0x35,0x38,
    0x33,0x70,0x78,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x6c,0x69,0x6e,0x65,0x63,
    0x61,0x70,0x3a,0x62,0x75,0x74,0x74,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x6c,
    0x69,0x6e,0x65,0x6a,0x6f,0x69,0x6e,0x3a,0x6d,0x69,0x74,0x65,0x72,0x3b,0x73,0x74,
    0x72,0x6f,0x6b,0x65,0x2d,0x6f,0x70,0x61,0x63,0x69,0x74,0x79,0x3a,0x31,0x22,0x0a,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x64,0x3d,0x22,0x6d,0x20,
    0x33,0x35,0x2e,0x38,0x39,0x38,0x33,0x37,0x2c,0x31,0x37,0x30,0x2e,0x36,0x30,0x35,
    0x34,0x35,0x20,0x31,0x33,0x2e,0x33,0x35,0x36,0x37,0x39,0x38,0x2c,0x31,0x33,0x2e,
    0x33,0x36,0x36,0x39,0x33,0x20,0x33,0x33,0x2e,0x31,0x38,0x33,0x30,0x30,0x31,0x2c,
    0x2d,0x35,0x34,0x2e,0x38,0x33,0x34,0x30,0x39,0x20,0x7a,0x22,0x0a,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x69,0x64,0x3d,0x22,0x70,0x61,0x74,0x68,
    0x33,0x31,0x34,0x31,0x22,0x2f,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x2f,
    0x67,0x3e,0x0a,0x20,0x20,0x20,0x20,0x3c,0x2f,0x67,0x3e,0x0a,0x20,0x20,0x3c,0x2f,
    0x67,0x3e,0x0a,0x3c,0x2f,0x73,0x76,0x67,0x3e,0x0a,
  };

 public:
  MISSILE_WEAPON_W_SVG() { }

  QImage image(int w, int h) {
    if (image_.isNull() || image_.width() != w || image_.height() != h) {
      QSvgRenderer renderer(QByteArray(reinterpret_cast<char *>(data_), 2250));

      image_ = QImage(w, h, QImage::Format_ARGB32);
      image_.fill(0);

      QPainter painter(&image_);
      renderer.render(&painter);
    }

    return image_;
  }

 private:
  QImage image_;
};

static MISSILE_WEAPON_W_SVG s_MISSILE_WEAPON_W_SVG;

#endif
