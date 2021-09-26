template <typename Concept, typename... Args>
struct TModels
{
	template <typename... Ts>
	static char (&Resolve(decltype(&Concept::template Requires<Ts...>)*))[2];

	template <typename... Ts>
	static char (&Resolve(...))[1];

	static constexpr bool Value = sizeof(Resolve<Args...>(0)) == 2;
};
