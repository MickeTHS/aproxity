#pragma once

struct Sort {
    virtual bool operator()(const void* lhv, const void* rhv) = 0;
};

struct SortFloatAsc : public Sort {
    virtual bool operator()(const void* lhv, const void* rhv) {
        return (*(float*)lhv) < (*(float*)rhv);
    }
};

struct SortFloatDesc : public Sort {
    virtual bool operator()(const void* lhv, const void* rhv) {
        return (*(float*)lhv) > (*(float*)rhv);
    }
};
