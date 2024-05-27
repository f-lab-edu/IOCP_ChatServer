#include "ServerService.h"

void ServerService::Open()
{
    Service::Open();
    _listener = std::make_shared<Listener>();
    
}
