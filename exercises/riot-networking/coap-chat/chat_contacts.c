#include <string.h>

#include "chat_contacts.h"

chat_contact_t chat_contacts[] = {
    { "grupo1", "fe80::d1c1:6d6c:ab0d:1336" },
    { "grupo2", "fe80::d1c1:6d52:ab43:1336" },
    { "grupo3", "fe80::47f7:6d43:828e:3332" },
    { "grupo4", "fe80::d1c1:6d42:ab00:1336" },
    { "grupo5", "fe80::d1c1:6d73:ab6a:1336" },
    { "leandro", "fe80::47f7:6d5a:8290:3332" },
    { "todos", "ff02::1" },
};

chat_contact_t *chat_contacts_get_by_nickname(char *nickname)
{
    chat_contact_t *dst = NULL;
    for (unsigned i = 0; i < chat_contacts_get_amount(); i++) {
        if (!strcmp(nickname, chat_contacts[i].nickname)) {
            dst = &chat_contacts[i];
        }
    }
    return dst;
}

chat_contact_t *chat_contacts_get_by_addr(char *addr)
{
    chat_contact_t *dst = NULL;
    for (unsigned i = 0; i < chat_contacts_get_amount(); i++) {
        if (!strcmp(addr, chat_contacts[i].addr)) {
            dst = &chat_contacts[i];
        }
    }
    return dst;
}

inline unsigned chat_contacts_get_amount(void)
{
    return sizeof(chat_contacts)/sizeof(chat_contacts[0]);
}
