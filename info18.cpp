#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <string>


class Matriz {
private:
    std::size_t m_filas;
    std::size_t m_columnas;
    std::vector<double> m_datos; // Almacenamiento contiguo unidimensional 

    
    [[nodiscard]] std::size_t mapear_indice(std::size_t f, std::size_t c) const {
        if (f >= m_filas || c >= m_columnas) {
            throw std::out_of_range(
                "Índice fuera de rango: acceso en (" + std::to_string(f) + ", " + 
                std::to_string(c) + ") para matriz de dimensiones " + 
                std::to_string(m_filas) + "x" + std::to_string(m_columnas) + "."
            );
        }
        return (f * m_columnas) + c;
    }

public:
    
    explicit Matriz(std::size_t filas, std::size_t columnas, double valor_inicial = 0.0)
        : m_filas(filas), 
          m_columnas(columnas), 
          m_datos(filas * columnas, valor_inicial) {
        if (filas == 0 || columnas == 0) {
            throw std::invalid_argument("Las dimensiones de la matriz deben ser mayores a cero.");
        }
    }

    ~Matriz() = default;
    Matriz(const Matriz&) = default;
    Matriz& operator=(const Matriz&) = default;
    Matriz(Matriz&&) noexcept = default;
    Matriz& operator=(Matriz&&) noexcept = default;

    
    [[nodiscard]] std::size_t filas() const noexcept { return m_filas; }
    [[nodiscard]] std::size_t columnas() const noexcept { return m_columnas; }

    [[nodiscard]] double& at(std::size_t f, std::size_t c) {
        return m_datos[mapear_indice(f, c)];
    }

    [[nodiscard]] const double& at(std::size_t f, std::size_t c) const {
        return m_datos[mapear_indice(f, c)];
    }

    void rellenar(double valor) noexcept {
        std::fill(m_datos.begin(), m_datos.end(), valor);
    }

 
     
    [[nodiscard]] Matriz operator+(const Matriz& otra) const {
        if (m_filas != otra.m_filas || m_columnas != otra.m_columnas) {
            throw std::invalid_argument(
                "Error en Suma: Dimensiones incompatibles. Se intento sumar (" + 
                std::to_string(m_filas) + "x" + std::to_string(m_columnas) + ") con (" + 
                std::to_string(otra.m_filas) + "x" + std::to_string(otra.m_columnas) + ")."
            );
        }

        Matriz resultado(m_filas, m_columnas);
        for (std::size_t i = 0; i < m_datos.size(); ++i) {
            resultado.m_datos[i] = m_datos[i] + otra.m_datos[i];
        }
        return resultado;
    }


    
    [[nodiscard]] Matriz transpuesta() const {
        Matriz resultado(m_columnas, m_filas);
        for (std::size_t i = 0; i < m_filas; ++i) {
            for (std::size_t j = 0; j < m_columnas; ++j) {
                resultado.at(j, i) = this->at(i, j);
            }
        }
        return resultado;
    }

   
    [[nodiscard]] Matriz operator*(const Matriz& otra) const {
        if (m_columnas != otra.m_filas) {
            throw std::invalid_argument(
                "Error en Multiplicacion: Dimensiones incompatibles. Columnas de A (" + 
                std::to_string(m_columnas) + ") != Filas de B (" + 
                std::to_string(otra.m_filas) + ")."
            );
        }

        Matriz resultado(m_filas, otra.m_columnas, 0.0);
        for (std::size_t i = 0; i < m_filas; ++i) {
            for (std::size_t k = 0; k < m_columnas; ++k) {
                const double elem_a = this->at(i, k);
                for (std::size_t j = 0; j < otra.m_columnas; ++j) {
                    resultado.at(i, j) += elem_a * otra.at(k, j);
                }
            }
        }
        return resultado;
    }

    
    void imprimir(const std::string& titulo = "") const {
        if (!titulo.empty()) {
            std::cout << titulo << " [" << m_filas << "x" << m_columnas << "]:\n";
        }
        for (std::size_t i = 0; i < m_filas; ++i) {
            std::cout << "  [ ";
            for (std::size_t j = 0; j < m_columnas; ++j) {
                std::cout << std::setw(6) << std::fixed << std::setprecision(1) << at(i, j) << " ";
            }
            std::cout << "]\n";
        }
    }
    

};

int main() {
    // Tus pruebas y llamadas a Matriz
    Matriz A(2, 3, 1.0);
    Matriz B(2, 3, 2.0);

    Matriz C = A + B;
    C.imprimir("Resultado");

    return 0;
}