/*
 * main.cpp
 *
 * Created on: 03-Aug-2011
 * Author: Shadab Ansari
 */
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <vector>

struct block {
	bool flagged;
	bool opened;
	bool mine;

	block() :
		flagged(false), opened(false), mine(false) {
	}
};

class grid {
private:
	static const int neighbour_[8][2];

	std::vector<block> blocks_;
	std::vector<int> adjacency_;

	const int width_;
	const int height_;

	int mines_;
	int flagged_;
	int opened_;

public:
	grid(int width, int height);
	~grid();

	int width() const {
		return width_;
	}

	int height() const {
		return height_;
	}

	int mines() const {
		return mines_;
	}

	int flagged_mines() const {
		return flagged_;
	}

	int opened_mines() const {
		return opened_;
	}

	void open(int x, int y);
	void flag(int x, int y);
	void unflag(int x, int y);

	template<typename InputIterator>
	void build(InputIterator start, InputIterator end);

private:
	bool is_valid(int x, int y) const;
	void open_adjacent(int x, int y);
	void build_adjacent(int x, int y);
	void clear();
};

const int grid::neighbour_[8][2] = {
		{-1, -1}, {+0, -1}, {+1, -1},
		{-1, +0},           {+1, +0},
		{-1, +1}, {+0, +1}, {+1, +1}
};

grid::grid(int width, int height)
	: width_(width), height_(height), mines_(0), flagged_(0), opened_(0) {

	if (width < 5 || height < 5) {
		throw std::out_of_range("Invalid width or height value.");
	}

	blocks_.assign(width_ * height_, block());
	adjacency_.assign(width_ * height_, 0);
}

grid::~grid() {

	std::cout << "\n:: Begin Grid Layout\n\n";

	for (size_t i = 0; i < blocks_.size(); ++i) {
		if (blocks_[i].mine) {
			std::cout << "\033[0;31m" << " [X]" << "\033[m";
		} else {
			std::cout << " [" << adjacency_[i] << "]";
		}
		if ((i + 1) % width_ == 0) {
			std::cout << '\n';
		}
	}

	std::cout << "\n:: End Grid Layout\n\n";
}


template<typename InputIterator>
void grid::build(InputIterator start, InputIterator end) {

	clear();

	while (start != end) {
		const int idx = *start;
		if (idx >= width_ * height_) {
			throw std::out_of_range("Invalid grid index value.");
		}
		if (++mines_ >= width_ * height_) {
			throw std::out_of_range("Too many mines.");
		}
		if (!blocks_[idx].mine) {
			blocks_[idx].mine = true;
			++mines_;
			build_adjacent(idx % width_, idx / width_);
		}
		++start;
	}
}

void grid::build_adjacent(int x, int y) {

	for (size_t i = 0; i < 8; ++i) {

		const int adj_x = x + neighbour_[i][0];
		const int adj_y = y + neighbour_[i][1];

		if (!is_valid(adj_x, adj_y)) {
			continue;
		}
		++adjacency_[(adj_y * width_) + adj_x];
	}
}

void grid::clear() {

	mines_   = 0;
	flagged_ = 0;
	opened_  = 0;

	blocks_.assign(blocks_.size(), block());
	adjacency_.assign(adjacency_.size(), 0);
}

bool grid::is_valid(int x, int y) const {

	return (x >= 0 && x < width_) && (y >= 0 && y < height_);
}

void grid::open(int x, int y) {

	const int idx = (y * width_) + x;
	if (!is_valid(x, y) || blocks_[idx].opened) {
		return;
	}

	blocks_[idx].opened = true;
	++opened_;

	if (!adjacency_[idx]) {
		open_adjacent(x, y);
	}
}

void grid::open_adjacent(int x, int y) {

	for (size_t i = 0; i < 8; ++i) {
		open(x + neighbour_[i][0], y + neighbour_[i][1]);
	}
}

void grid::flag(int x, int y) {

	if (!is_valid(x, y)) {
		return;
	}

	const int idx = (y * width_) + x;
	blocks_[idx].flagged = true;
	++flagged_;
}

void grid::unflag(int x, int y) {

	if (!is_valid(x, y)) {
		return;
	}

	const int idx = (y * width_) + x;
	blocks_[idx].flagged = false;
	--flagged_;
}


class minefield {
private:
	grid grid_;

public:
	minefield(int mines, int width, int height);
	~minefield();

	int width() const {
		return grid_.width();
	}

	int height() const {
		return grid_.height();
	}

	int mines() const {
		return grid_.mines();
	}

	int flagged_mines() const {
		return grid_.flagged_mines();
	}

	int opened_mines() const {
		return grid_.opened_mines();
	}

	void open(int x, int y);
	void flag(int x, int y);
	void unflag(int x, int y);

	void register_callback(EVENT_TYPE type);

	enum EVENT_TYPE {FLAG, UNFLAG, OPEN};
};

minefield::minefield(int mines, int width, int height) :
	grid_(width, height) {

	std::vector<int> positions(width * height, 0);

	for (size_t i = 0; i < positions.size(); ++i) {
		positions[i] = i;
	}
	std::srand(std::time(0));
	std::random_shuffle(positions.begin(), positions.end());

	grid_.build(positions.begin(), positions.begin() + mines);
}

void minefield::open(int x, int y) {

	grid_.open(x, y);
}

void minefield::flag(int x, int y) {

	grid_.flag(x, y);
}

void minefield::unflag(int x, int y) {

	grid_.unflag(x, y);
}

int main() {

	minefield field(10, 9, 9);

	return 0;
}
