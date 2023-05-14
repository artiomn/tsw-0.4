

Object::Object(Object &&other) :
    name_(std::move(other.name_)), fields_(std::move(fields_))
{

}


Object &Object::operator=(Object &&other)
{
    std::swap(fields_, other.fields_);
    std::swap(name_, other.name_);
}


Object::Object(const String &name) : name_(name)
{

}


Object::Object(String &&name) : name_(std::move(name))
{

}


Object::Object(const String &name, const NameValueMap &fields) : name_(name), fields_(fields)
{

}


Object::Object(String &&name, const NameValueMap &fields) : name_(std::move(name)), fields_(fields)
{

}


Object::Object(const String &name, NameValueMap &&fields) : name_(name), fields_(std::move(fields))
{

}


Object::Object(String &&name, NameValueMap &&fields) : name_(std::move(name)), fields_(std::move(fields))
{

}


const String& Object::get_name() const
{
    return name_;
}


const NameValueMap& Object::get_fields() const
{
    return fields_;
}


bool Object::operator<(const Object &other) const noexcept
{
    return name_ < other.name_;
}


bool Object::operator>(const Object &other) const noexcept
{
    return name_ > other.name_;
}


bool Object::operator<=(const Object &other) const noexcept
{
    return name_ <= other.name_;
}


bool Object::operator>=(const Object &other) const noexcept
{
    return name_ >= other.name_;
}


bool Object::operator==(const Object &other) const noexcept
{
    return name_ == other.name_;
}


bool Object::operator!=(const Object &other) const noexcept
{
    return !(*this == other);
}
