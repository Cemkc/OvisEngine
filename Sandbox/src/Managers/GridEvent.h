#pragma once

enum GridEventType
{
	FillEnd
};

#define GRID_EVENT_CLASS_TYPE(type) static GridEventType GetStaticType() { return GridEventType::##type; }\
								virtual GridEventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

class GridEvent
{
public:
	bool Handled = false;

	virtual GridEventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual std::string ToString() const { return GetName(); }
};

class FillEndEvent : public GridEvent
{
public:
	FillEndEvent() {}

	GRID_EVENT_CLASS_TYPE(FillEnd)
};
