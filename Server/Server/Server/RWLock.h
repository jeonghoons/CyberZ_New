#pragma once
#include "pch.h"
#include <shared_mutex>

class RWLock {
public:
    // �б�� RAII ��
    class ReadGuard {
    public:
        ReadGuard(RWLock& lock) : _lock(lock) {
            _lock._mutex.lock_shared();  // �б� �� ȹ��
        }
        ~ReadGuard() {
            _lock._mutex.unlock_shared(); // �б� �� ����
        }
    private:
        RWLock& _lock;
    };

    // ����� RAII ��
    class WriteGuard {
    public:
        WriteGuard(RWLock& lock) : _lock(lock) {
            _lock._mutex.lock();  // ���� �� ȹ��
        }
        ~WriteGuard() {
            _lock._mutex.unlock(); // ���� �� ����
        }
    private:
        RWLock& _lock;
    };

private:
    std::shared_mutex _mutex; // ���� mutex
};

