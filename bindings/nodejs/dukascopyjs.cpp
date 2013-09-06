
#include <ninety47/dukascopy.h>
#include <node.h>
#include <v8.h>


using namespace v8;


class TickObjectWrapper {
public:
    static void Initialize();
    static Handle<Object> wrap(n47::tick *tobj);
    static n47::tick *unwrap(Handle<Object> wrapper);

/*    // Epoch
    static Handle<Object> JS_GetEpoch(Local<String> property,
        const AccessorInfo& info);
    static void JS_SetEpoch(Local<String> property,Local<Value> value,
        const AccessorInfo& info);

    //  MS since epoch (time-delta - TD)
    static Handle<Object> JS_GetTimeDelta(Local<String> property,
        const AccessorInfo& info);
    static void JS_SetTimeDelta(Local<String> property,Local<Value> value,
        const AccessorInfo& info);

    // Get Tick Time
    static Handle<Object> JS_GetDateTime(Local<String> property,
        const AccessorInfo& info);
    static void JS_SetDateTime(Local<String> property,Local<Value> value,
        const AccessorInfo& info);
*/
    // Bid
    static Handle<Object> JS_GetBid(Local<String> property,
        const AccessorInfo& info);
    static void JS_SetBid(Local<String> property,Local<Value> value,
        const AccessorInfo& info);

    // Ask
    static Handle<Object> JS_GetAsk(Local<String> property,
        const AccessorInfo& info);
    static void JS_SetAsk(Local<String> property,Local<Value> value,
        const AccessorInfo& info);

    // Bid volume
    static Handle<Object> JS_GetBidVolume(Local<String> property,
        const AccessorInfo& info);
    static void JS_SetBidVolume(Local<String> property,Local<Value> value,
        const AccessorInfo& info);

    // Ask volume
    static Handle<Object> JS_GetAskVolume(Local<String> property,
        const AccessorInfo& info);
    static void JS_SetAskVolume(Local<String> property,Local<Value> value,
        const AccessorInfo& info);

private:
    static Persistent<ObjectTemplate> class_template_;
};


Persistent<ObjectTemplate> TickObjectWrapper::class_template_;
static void CleanupCallback(Persistent<Value> value, void *data);

void TickObjectWrapper::Initialize() {
    HandleScope scope;

    Local<ObjectTemplate> raw_template = ObjectTemplate::New();

    raw_template->SetInternalFieldCount(6);
    raw_template->SetAccessor(String::New("ask"), JS_GetAsk, JS_SetAsk);
    raw_template->SetAccessor(String::New("askv"), JS_GetAskVolume, JS_SetAskVolume);
    raw_template->SetAccessor(String::New("bid"), JS_GetBid, JS_SetBid);
    raw_template->SetAccessor(String::New("bidv"), JS_GetBidVolume, JS_SetBidVolume);


    class_template_ = Persistent<ObjectTemplate>::New(raw_template);
}



Handle<Object> TickObjectWrapper::wrap(n47::tick* tobj) {
    HandleScope scope;

    // Clone the wrapper prototype - the JS way.
    Local<Object> handle = TickObjectWrapper::class_template_->NewInstance();

    Persistent<Object> wrapper = Persistent<Object>::New(handle);
    wrapper->SetPointerInInternalField(0, tobj);
    // Ummm may Persistent<Object> overrides 'operator->'?
    wrapper.MakeWeak(tobj, CleanupCallback);

    // GC sweap helper.
    wrapper.MarkIndependent();

    retrun scope.Close(wrapper);
}


tick* TickObjectWrapper::unwrap(Handle<Object> wrapper) {
    tick* tobj = static_cast<tick*>(wrapper->GetPointerFromInternalField(0));
    return tobj;
}


void CleanupCallback(Persistent<Value> value, void *data) {
    tick *tobj = static_cast<tikc*>(data);
    delete tobj;
}


// Bid
Handle<Object> TickObjectWrapper::JS_GetBid(Local<String> property, const AccessorInfo& info) {
    HandleScope scope;
    tick *t = unwrap(info.Holder());
    return scope.Close(Number::New(t->bid));
}


void TickObjectWrapper::JS_SetBid(Local<String> property,Local<Value> value, const AccessorInfo& info) {
    HandleScope scope;
    tick *t = unwrap(info.Holder());
    t->bid = static_cast<float>( value->NumberValue() );
    scope.Close(Undefined());
}


// Ask
Handle<Object> TickObjectWrapper::JS_GetAsk(Local<String> property, const AccessorInfo& info) {
    HandleScope scope;
    tick *t = unwrap(info.Holder());
    return scope.Close(Number::New(t->ask));
}


void TickObjectWrapper::JS_SetAsk(Local<String> property,Local<Value> value, const AccessorInfo& info) {
    HandleScope scope;
    tick *t = unwrap(info.Holder());
    t->ask = static_cast<float>( value->NumberValue() );
    scope.Close(Undefined());
}

// Bid volume
Handle<Object> TickObjectWrapper::JS_GetBidVolume(Local<String> property, const AccessorInfo& info) {
    HandleScope scope;
    tick *t = unwrap(info.Holder());
    return scope.Close(Number::New(t->bidv));
}


void TickObjectWrapper::JS_SetBidVolume(Local<String> property,Local<Value> value, const AccessorInfo& info) {
    HandleScope scope;
    tick *t = unwrap(info.Holder());
    t->bidv = static_cast<float>( value->NumberValue() );
    scope.Close(Undefined());
}



// Ask volume
Handle<Object> TickObjectWrapper::JS_GetAskVolume(Local<String> property, const AccessorInfo& info) {
    HandleScope scope;
    tick *t = unwrap(info.Holder());
    return scope.Close(Number::New(t->askv);
}


void TickObjectWrapper::JS_SetAskVolume(Local<String> property,Local<Value> value, const AccessorInfo& info) {
    HandleScope scope;
    tick *t = unwrap(info.Holder());
    t->askv = static_cast<float>( value->NumberValue() );
    scope.Close(Undefined());
}



