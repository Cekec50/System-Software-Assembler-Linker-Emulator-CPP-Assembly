#include "../inc/linked_list.hpp"


Node::Node(MappedSection value) {
    data = value;
    prev = nullptr;
    next = nullptr;
}   


MappedSectionDLList::MappedSectionDLList() {
    head = nullptr;
    tail = nullptr;
}

void MappedSectionDLList::insertFront(MappedSection value) {
  Node* newNode = new Node(value);

  if (head == nullptr) {
      head = newNode;
      tail = newNode;
  } else {
      newNode->next = head;
      head->prev = newNode;
      head = newNode;
  }
}

void MappedSectionDLList::insertEnd(MappedSection value) {
  Node* newNode = new Node(value);

  if (head == nullptr) {
      head = newNode;
      tail = newNode;
  } else {
      tail->next = newNode;
      newNode->data.addressStart = tail->data.addressStart + tail->data.sectionSize;
      newNode->prev = tail;
      tail = newNode;
  }
}

void MappedSectionDLList::removeFront() {
  if (head == nullptr) {
      std::cout << "List is empty. Cannot remove from front." << std::endl;
      return;
  }

  Node* temp = head;

  if (head == tail) {
      head = nullptr;
      tail = nullptr;
  } else {
      head = head->next;
      head->prev = nullptr;
  }

  delete temp;
}

void MappedSectionDLList::removeEnd() {
  if (tail == nullptr) {
      std::cout << "List is empty. Cannot remove from end." << std::endl;
      return;
  }

  Node* temp = tail;

  if (head == tail) {
      head = nullptr;
      tail = nullptr;
  } else {
      tail = tail->prev;
      tail->next = nullptr;
  }

  delete temp;
}

    

void MappedSectionDLList::insertToSection(MappedSection value) {
    
  if (head == nullptr) {
      insertFront(value);
      return;
  }

  Node* newNode = new Node(value);

  Node* current = tail;

  while (current != nullptr && current->data.sectionName == value.sectionName ) {
      current = current->prev;
  }

  if (current != nullptr ) {
      newNode->prev = current;
      newNode->data.addressStart = current->data.addressStart + current->data.sectionSize;
      newNode->next = current->next;

      if (current->next != nullptr) {
          current->next->prev = newNode;
      } else {
          tail = newNode;
      }

      current->next = newNode;
  } else {
      insertEnd(value);
  }
}

void MappedSectionDLList::printList(){
  Node* current = head;

        while (current != nullptr) {
            cout << "[" << current->data.addressStart << ": " << current->data.sectionName<<"(" <<current->data.fileIndex << ")" << "{" << current->data.sectionSize << "}]-->";
            current = current->next;
        }

        cout << endl;
}
