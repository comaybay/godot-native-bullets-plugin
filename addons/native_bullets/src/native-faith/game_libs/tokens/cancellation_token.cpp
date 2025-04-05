#include "cancellation_token.h"

void CancellationToken::_bind_methods() {
    ClassDB::bind_method(D_METHOD("is_canceled"), &CancellationToken::is_canceled);
    ClassDB::bind_method(D_METHOD("cancel"), &CancellationToken::cancel);
    ClassDB::bind_static_method("CancellationToken", D_METHOD("create_cancelled_token"), &CancellationToken::create_cancelled_token);
}

bool CancellationToken::is_canceled() const {
    return _canceled;
}

void CancellationToken::cancel() {
    _canceled = true;
}

Ref<CancellationToken> CancellationToken::create_cancelled_token() {
    Ref<CancellationToken> token;
    token.instantiate();
    token->cancel();
    return token;
}
