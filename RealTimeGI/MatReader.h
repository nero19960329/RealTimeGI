#pragma once

#include <mat.h>

#include <array>
#include <iostream>
#include <vector>

#include "Utils.h"

class MatReader {
	std::string filename;

public:
	MatReader(const std::string &_name) :
		filename(_name) {}

	std::vector<float> readNN() const {
		MATFile *pmat;

		pmat = matOpen(filename.c_str(), "r");
		if (!pmat) error_exit("Open file error!\n");

		mxArray *pa = matGetVariable(pmat, "nnwb");
		if (!pa) error_exit("Null mat file!\n");

		std::vector<float> res;

		mwSize num = mxGetNumberOfElements(pa);
		pushFloats(res, mxGetCell(pa, 6));
		pushFloats(res, mxGetCell(pa, 7));

		std::array<int, 15> usedMap;
		int cnt = 0;
		rep(i, 15) {
			if (res[i] == res[i + 15]) {
				res[i] = -100.0;
				res[i + 15] = 100.0;
				usedMap[i] = -1;
			} else {
				usedMap[i] = cnt;
				++cnt;
			}
		}

		pushFloats(res, mxGetCell(pa, 8));
		pushFloats(res, mxGetCell(pa, 9));
		pushFloats_w1(res, mxGetCell(pa, 0), usedMap);

		pushFloats(res, mxGetCell(pa, 1));
		pushFloats(res, mxGetCell(pa, 2));
		pushFloats(res, mxGetCell(pa, 3));
		pushFloats_layer3(res, mxGetCell(pa, 4));
		pushFloats_layer3(res, mxGetCell(pa, 5));

		mxDestroyArray(pa);
		matClose(pmat);

		return res;
	}

private:
	void pushFloats(std::vector<float> &vec, mxArray *parr) const {
		real_t *p = mxGetPr(parr);
		int rows = mxGetM(parr), cols = mxGetN(parr);

		if (!p) error_exit("Null mat file!\n");
		rep(i, rows) rep(j, cols) vec.push_back(p[j * rows + i]);
	}

	void pushFloats_layer3(std::vector<float> &vec, mxArray *parr) const {
		real_t *p = mxGetPr(parr);
		int rows = mxGetM(parr), cols = mxGetN(parr);

		if (!p) error_exit("Null mat file!\n");
		if (rows == 3) rep(i, rows) rep(j, cols) vec.push_back(p[j * rows + i]);
		else if (rows == 2) {
			rep(i, 3) rep(j, cols) {
				if (!i) vec.push_back(0.0);
				else vec.push_back(p[j * rows + i - 1]);
			}
		}
	}

	void pushFloats_w1(std::vector<float> &vec, mxArray *parr, std::array<int, 15> &usedMap) const {
		real_t *p = mxGetPr(parr);
		int rows = mxGetM(parr), cols = mxGetN(parr);

		if (!p) error_exit("Null mat file!\n");

		rep(i, rows) rep(j, 15) {
			if (usedMap[j] >= 0) vec.push_back(p[usedMap[j] * rows + i]);
			else vec.push_back(0.0f);
		}
	}
};