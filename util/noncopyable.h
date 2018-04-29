//
// Created by Rolrence on 4/6/2018.
//

#ifndef NATIVEJS_NONCOPYABLE_H
#define NATIVEJS_NONCOPYABLE_H

class noncopyable {
protected:
    noncopyable() {}

    ~noncopyable() {}

private:
    noncopyable(const noncopyable &);

    noncopyable &operator=(const noncopyable &);
};

#endif //NATIVEJS_NONCOPYABLE_H
