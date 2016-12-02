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

		repa(i, 1, 6) pushFloats(res, mxGetCell(pa, i));

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