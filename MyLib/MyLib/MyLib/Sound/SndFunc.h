#pragma once
#include <string>
#include <vector>

namespace snd
{
	// RIFF
	struct RIFF {
		//ID
		unsigned char id[4];
		//�T�C�Y
		long size;
		//�^�C�v
		unsigned char type[4];
	};

	// FMT
	struct FMT {
		//ID
		unsigned char id[4];
		//�T�C�Y
		long size;
		//�^�C�v
		short type;
		//�`�����l��
		short channel;
		//�T���v�����O���g��
		long sample;
		//�o�C�g
		long byte;
		//�u���b�N�T�C�Y
		short block;
		//�ʎq���r�b�g��
		short bit;
	};

	// DATA
	struct DATA {
		//ID
		std::string id;
		//�T�C�Y
		long size;
	};


	// RIFF�̓ǂݍ���
	int LoadRIFF(RIFF& riff, FILE* file);

	// FMT�̓ǂݍ���
	int LoadFMT(FMT& fmt, FILE* file);

	// DATA�̓ǂݍ���
	int LoadDATA(DATA& data, FILE* file);

	// 8�r�b�g�g�`�̓ǂݍ���
	int LoadWave8(std::vector<float>& data, FILE* file);

	// 16�r�b�g�g�`�̓ǂݍ���
	int LoadWave16(std::vector<float>& data, FILE* file);
}