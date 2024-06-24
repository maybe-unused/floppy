#pragma once

#include <string_view>
#include <stdexcept>
#include <floppy/floppy.h>

/// \brief Namespace with types related to serialization and deserialization.
namespace floppy::serialization
{
  /// \brief Supported serialization formats.
  /// \headerfile floppy/serialization.h 
  /// \ingroup serialization
  enum class format : u8
  {
    json,         ///< JSON format (JavaScript Object Notation)
    bson,         ///< JSON-like binary format
    xml,          ///< XML format (Textual format used in many legacy projects)
    cbor,         ///< CBOR format (JSON-like binary format)
    flexbuffer,   ///< Schema-less version of flatbuffers, binary format
    msgpack,      ///< MessagePack format (JSON-like binary format)
    toml,         ///< Textual format with an emphasis on readability
    yaml          ///< Textual format with an emphasis on readability
  };

  /// \brief Implementation details.
  namespace detail
  {
    /// \brief Convert format to string.
    /// \note This function is used purely for implementation details, that's the reason it
    /// declared this way and not via <tt>fmt::formatter</tt> extension.
    [[maybe_unused]] constexpr auto to_string(format f) noexcept -> std::string_view
    {
      switch(f) {
        case format::json: return "json";
        case format::bson: return "bson";
        case format::xml: return "xml";
        case format::cbor: return "cbor";
        case format::flexbuffer: return "flexbuffer";
        case format::msgpack: return "msgpack";
        case format::toml: return "toml";
        case format::yaml: return "yaml";
        default: return "unknown";
      }
    }
  } // namespace detail

  /// \brief Exception thrown during serialization or deserialization.
  /// \headerfile floppy/serialization.h 
  /// \ingroup serialization
  /// \see serialize, deserialize
  class serialization_error : public std::runtime_error
  {
   public:
    /// \brief Constructs exception with given format and details.
    /// \param f Serialization format.
    /// \param details A detailed description of the error.
    explicit serialization_error(format f, std::string_view details)
      : std::runtime_error(fmt::format("error during serialization or deserialization in format `{}`: {}",
          detail::to_string(f),
          details
        ))
    {}

    /// \brief Constructs exception with given format.
    /// \details Same as \ref serialization_error(format, std::string_view), but omitting details.
    /// \param f Serialization format.
    /// \see serialization_error
    explicit serialization_error(format f)
      : std::runtime_error(fmt::format("error during serialization or deserialization in format `{}`",
          detail::to_string(f)
        ))
    {}
  };

  /// \brief Template base function for serialization given type.
  /// \headerfile floppy/serialization.h 
  /// \ingroup serialization
  /// \tparam F Format type. Must be value of enum \ref floppy::serialization::format.
  /// \tparam T Type to serialize.
  /// \tparam C Character type. Defaults to char.
  /// \param value Value to serialize.
  /// \return Value serialized as string in given format.
  /// \throws serialization_error if deserialization fails.
  /// \see deserialize
  /// \see serialization_error
  /// \see serializable, serializable_and_deserializable
  template <auto F, typename T, typename C = char>
  requires std::is_same_v<decltype(F), format>
  [[maybe_unused]] auto serialize(T const& value) -> std::basic_string<C> = delete;

  /// \brief Template base function for deserialization given type.
  /// \headerfile floppy/serialization.h 
  /// \ingroup serialization
  /// \tparam F Format type. Must be value of enum \ref floppy::serialization::format.
  /// \tparam T Type to deserialize.
  /// \tparam C Character type. Defaults to char.
  /// \param value Value to deserialize.
  /// \return Value deserialized from string in given format.
  /// \throws serialization_error if deserialization fails.
  /// \see serialize
  /// \see serialization_error
  /// \see deserializable, serializable_and_deserializable
  template <auto F, typename T, typename C = char>
  requires std::is_same_v<decltype(F), format>
  [[maybe_unused]] auto deserialize(std::basic_string<C> const& value) -> T = delete;

  /// \brief Concept describing serializable types.
  /// \headerfile floppy/serialization.h 
  /// \ingroup serialization
  /// \tparam F Format type. Must be value of enum \ref floppy::serialization::format.
  /// \tparam T Type to serialize.
  /// \tparam C Character type. Defaults to char.
  /// \see deserializable, serializable_and_deserializable
  /// \see serialize, deserialize
  template <auto F, typename T, typename C = char>
  concept serializable = requires(T const& t) {
    { serialize<F, T, C>(t) } -> std::convertible_to<std::basic_string<C>>;
  };

  /// \brief Concept describing deserializable types.
  /// \headerfile floppy/serialization.h 
  /// \ingroup serialization
  /// \tparam F Format type. Must be value of enum \ref floppy::serialization::format.
  /// \tparam T Type to deserialize.
  /// \tparam C Character type. Defaults to char.
  /// \see serializable, serializable_and_deserializable
  /// \see serialize, deserialize
  template <auto F, typename T, typename C = char>
  concept deserializable = requires(std::basic_string<C> const& s) {
    { deserialize<F, T, C>(s) } -> std::convertible_to<T>;
  };

  /// \brief Concept describing serializable and deserializable types.
  /// \headerfile floppy/serialization.h 
  /// \ingroup serialization
  /// \tparam F Format type. Must be value of enum \ref floppy::serialization::format.
  /// \tparam T Type to serialize and deserialize.
  /// \tparam C Character type. Defaults to char.
  /// \see serializable, deserializable
  /// \see serialize, deserialize
  template <auto F, typename T, typename C = char>
  concept serializable_and_deserializable = serializable<F, T, C> and deserializable<F, T, C>;
} // namespace floppy::serialization

/// \defgroup serialization Serialization
/// \ingroup foundation
/// \brief Serialization-related functions.