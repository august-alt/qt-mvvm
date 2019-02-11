#include "google_test.h"
#include "commands.h"
#include "sessionitem.h"
#include "sessionmodel.h"
#include "taginfo.h"
#include "test_utils.h"
#include <QDebug>

using namespace ModelView;

class TestCommands : public ::testing::Test
{
public:
    ~TestCommands();
};

TestCommands::~TestCommands() = default;

//! Set item value through SetValueCommand command.

TEST_F(TestCommands, setValueCommand)
{
    SessionModel model;
    const int role = ItemDataRole::DISPLAY;

    // inserting single item
    auto item = model.insertNewItem("abc");
    EXPECT_FALSE(model.data(item, role).isValid());

    QVariant expected(42.0);
    auto command = std::make_unique<SetValueCommand>(item, expected, role);

    // executing command
    command->redo();
    EXPECT_TRUE(command->result()); // value was changed
    EXPECT_EQ(model.data(item, role), expected);

    // undoing command
    command->undo();
    EXPECT_TRUE(command->result()); // value was changed
    EXPECT_FALSE(model.data(item, role).isValid());
}

//! Set same item value through SetValueCommand command.

TEST_F(TestCommands, setSameValueCommand)
{
    SessionModel model;
    const int role = ItemDataRole::DISPLAY;

    // inserting single item
    auto item = model.insertNewItem("abc");
    QVariant expected(42.0);
    item->setData(expected, role);

    // command to set same value
    auto command = std::make_unique<SetValueCommand>(item, expected, role);

    // executing command
    command->redo();
    EXPECT_FALSE(command->result()); // value wasn't changed
    EXPECT_EQ(model.data(item, role), expected);

    // undoing command
    command->undo();
    EXPECT_FALSE(command->result()); // value wasn't changed
    EXPECT_EQ(model.data(item, role), expected);
}

//! Insert new item through InsertNewItemCommand command.

TEST_F(TestCommands, insertNewItemCommand)
{
    SessionModel model;

    // command to set same value
    auto command = std::make_unique<InsertNewItemCommand>("abc", model.rootItem(), 0, "");

    // executing command
    command->redo();
    EXPECT_EQ(model.rootItem()->childrenCount(), 1);
    EXPECT_EQ(command->result(), model.rootItem()->childAt(0));

    // undoing command
    command->undo();
    EXPECT_EQ(model.rootItem()->childrenCount(), 0);
    EXPECT_EQ(command->result(), nullptr);
}

//! Insert new item through InsertNewItemCommand command.

TEST_F(TestCommands, insertNewItemWithTagCommand)
{
    SessionModel model;

    // command to insert parent in the model
    auto command1 = std::make_unique<InsertNewItemCommand>("parent_model", model.rootItem(), 0, "");
    command1->redo(); // insertion

    auto parent = command1->result();
    parent->registerTag(TagInfo::universalTag("tag1"), /*set_as_default*/true);
    EXPECT_EQ(parent->childrenCount(), 0);

    // command to insert child
    auto command2 = std::make_unique<InsertNewItemCommand>("child_model", parent, 0, "tag1");
    command2->redo(); // insertion

    EXPECT_EQ(parent->childrenCount(), 1);
    EXPECT_EQ(parent->childAt(0), command2->result());

    // undoing command
    command2->undo();
    EXPECT_EQ(parent->childrenCount(), 0);
    EXPECT_EQ(nullptr, command2->result());
}

TEST_F(TestCommands, removeAtCommand)
{
    SessionModel model;
    auto item = model.insertNewItem("abc", model.rootItem(), 0, "");

    auto item_identifier = item->data(ItemDataRole::IDENTIFIER).value<std::string>();

    // command to insert parent in the model
    auto command = std::make_unique<RemoveAtCommand>(model.rootItem(), 0);
    command->redo(); // removal

    EXPECT_EQ(command->result(), true);
    EXPECT_EQ(model.rootItem()->childrenCount(), 0);

    // undo command
    command->undo();
    EXPECT_EQ(model.rootItem()->childrenCount(), 1);
    auto restored = model.rootItem()->childAt(0);
    EXPECT_EQ(restored->data(ItemDataRole::IDENTIFIER).value<std::string>(), item_identifier);
}

TEST_F(TestCommands, removeAtCommandChild)
{
    SessionModel model;
    auto parent = model.insertNewItem("abc", model.rootItem(), 0, "");
    parent->registerTag(TagInfo::universalTag("tag1"), /*set_as_default*/true);

    auto child1 = model.insertNewItem("child_model", parent, -1, "tag1");
    child1->setData(42.0, ItemDataRole::DATA);
    model.insertNewItem("child_model", parent, -1, "tag1");

    auto child1_identifier = child1->data(ItemDataRole::IDENTIFIER).value<std::string>();

    // command to remove one child
    auto command = std::make_unique<RemoveAtCommand>(parent, 0);
    command->redo(); // removal

    // check that one child was removed
    EXPECT_EQ(command->result(), true);
    EXPECT_EQ(parent->childrenCount(), 1);

    // undo command
    command->undo();
    EXPECT_EQ(parent->childrenCount(), 2);
    auto restored = parent->childAt(0);
    EXPECT_EQ(restored->data(ItemDataRole::IDENTIFIER).value<std::string>(), child1_identifier);

    // checking the data of restored item
    EXPECT_EQ(restored->data(ItemDataRole::DATA).toDouble(), 42.0);
}

TEST_F(TestCommands, removeAtCommandParentWithChild)
{
    SessionModel model;
    auto parent = model.insertNewItem("abc", model.rootItem(), 0, "");
    parent->registerTag(TagInfo::universalTag("tag1"), /*set_as_default*/true);

    auto child1 = model.insertNewItem("child_model", parent, -1, "tag1");
    child1->setData(42.0, ItemDataRole::DATA);

    auto parent_identifier = parent->data(ItemDataRole::IDENTIFIER).value<std::string>();
    auto child1_identifier = child1->data(ItemDataRole::IDENTIFIER).value<std::string>();

    // command to remove parent
    auto command = std::make_unique<RemoveAtCommand>(model.rootItem(), 0);
    command->redo(); // removal

    // check that one child was removed
    EXPECT_EQ(command->result(), true);
    EXPECT_EQ(model.rootItem()->childrenCount(), 0);

    // undo command
    command->undo();
    EXPECT_EQ(model.rootItem()->childrenCount(), 1);
    auto restored_parent = model.rootItem()->childAt(0);
    auto restored_child = restored_parent->childAt(0);

    EXPECT_EQ(restored_parent->data(ItemDataRole::IDENTIFIER).value<std::string>(), parent_identifier);
    EXPECT_EQ(restored_child->data(ItemDataRole::IDENTIFIER).value<std::string>(), child1_identifier);

    // checking the data of restored item
    EXPECT_EQ(restored_child->data(ItemDataRole::DATA).toDouble(), 42.0);
}

//! RemoveAtCommand in multitag context

//! FIXME restore

//TEST_F(TestCommands, removeAtCommandMultitag)
//{
//    SessionModel model;
//    auto parent = model.insertNewItem("abc", model.rootItem(), 0, "");
//    parent->registerTag(TagInfo::universalTag("tag1"));
//    parent->registerTag(TagInfo::universalTag("tag2"));

//    auto child1 = model.insertNewItem("child_model", parent, -1, "tag1");
//    child1->setData(41.0, ItemDataRole::DATA);

//    auto child2 = model.insertNewItem("child_model", parent, -1, "tag1");
//    child2->setData(42.0, ItemDataRole::DATA);

//    auto child3 = model.insertNewItem("child_model", parent, -1, "tag2");
//    child3->setData(43.0, ItemDataRole::DATA);

//    auto parent_identifier = parent->data(ItemDataRole::IDENTIFIER).value<std::string>();
//    auto child1_identifier = child1->data(ItemDataRole::IDENTIFIER).value<std::string>();
//    auto child2_identifier = child2->data(ItemDataRole::IDENTIFIER).value<std::string>();
//    auto child3_identifier = child3->data(ItemDataRole::IDENTIFIER).value<std::string>();

//    // command to remove parent
//    auto command = std::make_unique<RemoveAtCommand>(parent, 1);
//    command->redo(); // removal

//    // check that one child was removed
//    EXPECT_EQ(command->result(), true);
//    EXPECT_EQ(parent->childrenCount(), 2);

//    // undo command
//    command->undo();
//    EXPECT_EQ(model.rootItem()->childrenCount(), 3);
//    auto restored_parent = model.rootItem()->childAt(0);
//    auto restored_child2 = restored_parent->childAt(1);

//    EXPECT_EQ(restored_parent->data(ItemDataRole::IDENTIFIER).value<std::string>(), parent_identifier);
//    EXPECT_EQ(restored_child2->data(ItemDataRole::IDENTIFIER).value<std::string>(), child2_identifier);

//    // checking the data of restored item
//    EXPECT_EQ(restored_child2->data(ItemDataRole::DATA).toDouble(), 42.0);
//}