#ifndef CANCELLATION_TOKEN_H
#define CANCELLATION_TOKEN_H

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

class CancellationToken : public RefCounted {
    GDCLASS(CancellationToken, RefCounted);

protected:
    static void _bind_methods();

private:
    bool _canceled = false;

public:
    bool is_canceled() const;
    void cancel();
    
    static Ref<CancellationToken> create_cancelled_token();
};

#endif // CANCELLATION_TOKEN_H
