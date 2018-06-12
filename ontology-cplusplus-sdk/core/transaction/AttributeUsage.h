#ifndef ATTRIBUTEUSAGE_H
#define ATTRIBUTEUSAGE_H

enum class AttributeUsage
{
  Nonce = 0x00,
  Script = 0x20,
  DescriptionUrl = 0x81,
  Description = 0x90
};

AttributeUsage valueOf(int v)
{
  AttributeUsage ret;
  switch (v)
  {
  case 0x00:
    ret = AttributeUsage::Nonce;
    break;
  case 0x20:
    ret = AttributeUsage::Script;
    break;
  case 0x81:
    ret = AttributeUsage::DescriptionUrl;
    break;
  case 0x90:
    ret = AttributeUsage::Description;
    break;
  default:
    throw "IllegalArgumentException";
  }
  return ret;
}

#endif // !ATTRIBUTEUSAGE_H