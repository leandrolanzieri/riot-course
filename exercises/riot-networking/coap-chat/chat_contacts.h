typedef struct chat_contact {
    const char *nickname;
    const char *addr;
} chat_contact_t;

extern chat_contact_t chat_contacts[];

/**
 * @brief Get a contact by nickname
 *
 * @param nickname nickname to search by
 *
 * @return chat_contact_t* contact found
 * @return NULL if not found
 */
chat_contact_t *chat_contacts_get_by_nickname(char *nickname);

/**
 * @brief Get a contact by address
 *
 * @param addr address so search by
 *
 * @return chat_contact_t* contact found
 * @return NULL if not found
 */
chat_contact_t *chat_contacts_get_by_addr(char *addr);

/**
 * @brief Return the number of registered contacts
 * 
 * @return unsigned number of registered contacts
 */
unsigned chat_contacts_get_amount(void);
